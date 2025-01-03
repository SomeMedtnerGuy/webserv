/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:52:27 by nsouza-o          #+#    #+#             */
/*   Updated: 2025/01/03 18:56:24 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Location.hpp"

Location::Location(){}

Location::Location(const Location& src)
{
	*this = src;
}

Location& Location::operator=(const Location& src)
{
	if (this != &src)
	{
		_allowMethods = src._allowMethods;
		_autoindex = src._autoindex;
		_index = src._index;
		_return = src._return;
		_root = src._root;
	}
	return (*this);
}

Location::~Location(){}

/* Setters */

void Location::setSpecificPath(std::string& specificPath)
{
	_specificPath = specificPath;
}

void Location::setAllowMethods(std::vector<std::string>& allowMethods)
{
	
	checkSemicolonAtEnd(const_cast<const std::string&>(allowMethods[allowMethods.size() - 1]), 0, "Allow Methods"); /* check id block */
	
	std::vector<std::string> methods = allowMethods;
	methods[methods.size() - 1].erase(methods[methods.size() - 1].size() - 1);
	
	for (size_t i = 1; i < methods.size(); i++) /* check methods allowed */
	{
		if (methods[i] == "GET")
			continue;
		if (methods[i] == "POST")
			continue;
		if (methods[i] == "DELETE")
			continue;
		throw std::runtime_error("Invalid directive allow_methods in " + _specificPath + " location.");
	}
	methods.erase(methods.begin());
	_allowMethods = methods;
}

void Location::setAutoIndex(std::vector<std::string>& autoIndex)
{
	checkSemicolonAtEnd(const_cast<const std::string&>(autoIndex[autoIndex.size() - 1]), 0, "autoindex");
	
	std::vector<std::string> autoindex = autoIndex;
	if (autoIndex.size() != 2)
		throw std::runtime_error("Autoindex directive must have only one value in location block " + _specificPath + ".");
	autoindex[1].erase(autoindex[1].size() - 1);
	
	if (autoindex[1] == "on")
		_autoindex = true;
	else if (autoindex[1] == "off")
		_autoindex = false;
	else
		throw std::runtime_error("The value of autoindex directive must be on of off.");
}

void Location::setIndex(std::vector<std::string>& index)
{
	if (index.size() < 2)
		throw std::runtime_error("Host directive must have one value, at least.");
	
	checkSemicolonAtEnd(index[index.size() - 1], 0, "Index");
	
	for (size_t i = 1; i < index.size(); i++)
	{
		if (i == index.size() - 1)
		{
			std::string lastIndex = index[i];
			lastIndex.erase(lastIndex.size() - 1);
			if (lastIndex.find_first_not_of(' ') != std::string::npos)
				_index.push_back(lastIndex);
		}
		else
			_index.push_back(index[i]);
	}
}

void Location::setReturn(std::vector<std::string>& Return)
{
	if (Return.size() < 2)
		throw std::runtime_error("The Error Page directive must have a value.");
	if (Return.size() > 3)
		throw std::runtime_error("The Error Page directive must have a http code and a value.");
	
	checkSemicolonAtEnd(Return[Return.size() - 1], 0, "Return");
	std::string returnContent = Return[Return.size() - 1];
	returnContent.erase(Return.size() - 1);
	
	if (Return.size() == 3)
	{
		for (size_t j = 0; j < Return[1].size(); j++)
		{
			if (!isdigit(Return[1][j]))
				throw std::runtime_error("Invalid Return directive: Expected a number value in http code.");
		}
		int code = atoi(Return[1].c_str());
		if (code < 100 || code > 600)
			throw std::runtime_error("Invalid HTTP code in Return directive.");
		_return.push_back(Return[1]);
		_return.push_back(returnContent);			
	}
	else
	{
		_return.push_back("200");
		_return.push_back(returnContent);
	}
}

void Location::setRoot(std::vector<std::string>& root)
{
	if (root.size() != 2)
		throw std::runtime_error("Root directive must not have more than one value");

	checkSemicolonAtEnd(root[1], 0, "Root");
	std::string lastRootElement = root[1];
	lastRootElement.erase(lastRootElement.size() - 1);
	
	if (!isDirectory(lastRootElement))
			throw std::runtime_error("Root directive must have a directory path.");
	
	_root = lastRootElement;
}

/* Getters */
std::string Location::getSpecificPath() const
{
	return (_specificPath);
}

bool Location::getAllowMethods(std::string method) const
{
	for (size_t i = 0; i < _allowMethods.size(); i++)
	{
		if (!_allowMethods[i].compare(method))
			return (true);
	}
	return (false);
}

bool Location::getAutoIndex() const
{
	return (_autoindex);
}

size_t Location::getIndexSize() const
{
	return (_index.size());
}

std::string Location::getIndex(size_t indexNbr) const
{
	if (indexNbr > _index.size() -1)
		throw std::runtime_error("Index number out of range. Valid range: 0 to " + intToStr(_index.size() -1));

	return (_index[indexNbr]);
}

std::vector<std::string> Location::getReturn() const
{
	if (_return.size() == 0)
		return (std::vector<std::string>());
	return (_return);
}

std::string Location::getRoot() const
{
	return (_root);
}

/* - */

void Location::setLocationElements(std::string& element)
{
	void (Location::*SetFunct[5])(std::vector<std::string>&) = {
		&Location::setAllowMethods,
		&Location::setAutoIndex,
		&Location::setIndex,
		&Location::setReturn, 
		&Location::setRoot
		};
	
	std::vector<std::string> elementVector = splitServerStr(element);
	size_t nbr =  getLocationNbr(elementVector[0]);
	
	(this->*SetFunct[nbr])(elementVector);
}

size_t getLocationNbr(std::string element)
{
	std::string elementsKeys[5] = {
		"allow_methods",
		"autoindex", 
		"index", 
		"return", 
		"root", 
		};
	
	for (size_t i = 0; i < 5; ++i) {
		if (element == elementsKeys[i])
			return (i);		
	}
	throw std::runtime_error("Invalid element on Location block: " + element);
}
