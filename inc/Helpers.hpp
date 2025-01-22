/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helpers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:57:37 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/01/03 17:14:15 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_HPP
# define HELPERS_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <netinet/in.h>

class Helpers
{
private:
    Helpers();
    ~Helpers();
public:
    template<typename T>
    static std::string  ntostr(T number)
    {
    std::stringstream   converter;
    converter << number;
    return (converter.str());
    }
    
    static int  getFileLength(std::string filename);
    static int  getPortFromSocket(int sockfd);
};


#endif

/*
SETTINGS:
-serverName: 			"default"
-root: 					"./root"
-clientBodySizeLimit:	1000*1000
-index:					"index.html"
-errorPages:				<error>: "./.default/<error>.html" (where error is the error code)
-allowMethods:			GET, POST, DELETE (the enum items)
-autoindex:				false
-returnCode:			-1
-returnURL:				""				
*/
