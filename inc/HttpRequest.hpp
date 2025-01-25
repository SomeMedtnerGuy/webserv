/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 12:12:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/25 12:27:07 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_HPP
# define HTTP_REQUEST_HPP

# include "HttpMessage.hpp"
# include "utils.hpp" //for Method

class HttpRequest: public HttpMessage
{
private:
    Method			_method;
	std::string		_target;
	bool			_hasBody;

    HttpRequest(const HttpRequest& other);
    HttpRequest&    operator=(const HttpRequest& other);
public:
    void	setMethod(Method method);
	void	setTarget(std::string target);
	void	setHasBody(bool hasBody);

    Method		getMethod(void) const;
	std::string	getTarget(void) const;
	bool		getHasBody(void) const;
	    
    HttpRequest();
    ~HttpRequest();

    static Method	strToMethod(std::string str);
};

#endif
