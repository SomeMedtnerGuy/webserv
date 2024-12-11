/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:52:27 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/12/11 19:24:57 by nsouza-o         ###   ########.fr       */
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
		if (methods[i] == "PUT")
			continue;
		if (methods[i] == "HEAD")
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
	_index = index[0];
}

void Location::setReturn(std::vector<std::string>& Return)
{
	_return = Return[0];
}

void Location::setRoot(std::vector<std::string>& root)
{
	_root = root[0];
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

std::string Location::getIndex() const
{
	return (_index);
}

std::string Location::getReturn() const
{
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
