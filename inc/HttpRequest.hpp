/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 12:12:18 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/03 17:14:10 by ndo-vale         ###   ########.fr       */
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

    HttpRequest(const HttpRequest& other);
    HttpRequest&    operator=(const HttpRequest& other);
public:
    void	setMethod(Method method);
	void	setTarget(std::string target);

    Method				getMethod(void) const;
	std::string			getTarget(void) const;
    
    HttpRequest();
    ~HttpRequest();

    static Method	strToMethod(std::string str);
};

#endif
