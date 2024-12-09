/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joamonte <joamonte@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 22:13:15 by joamonte          #+#    #+#             */
/*   Updated: 2024/12/08 22:49:51 by joamonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include <vector>
#include <utility>
#include <limits>
#include <climits>
#include <csignal>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>


extern bool	g_shutdown;

//Key Words
#define ALLOW_M		"allow_methods"
#define AUTOID		"autoindex"
#define CGI_E		"cgi_ext"
#define CGI_P		"cgi_path"
#define MAX_SIZE	"client_max_body_size"
#define ERROR_P		"error_page"
#define	HOST		"host"
#define INDEX		"index"
#define LISTEN		"listen"
#define LOCATION	"location"
#define RETURN		"return"
#define ROOT		"root"
#define SERVER		"server"
#define SERVER_N	"server_name"
#define TRY 		"try_file"
#define UPLOAD		"upload_to"

// Default settings
#define DEFAULT_CONF		"system/default.conf"
#define EXT_CONF			".conf"
#define EXT_HTML			".html"
#define CGI_OUTPUT_FILE		".cgi_output"
#define PYTHON_EXT			".py"
#define PYTHON_BIN			"/usr/bin/python3"
#define PYTHON_VERSION		"python3"
#define PHP_EXT				".php"
#define MAX_PORT			65535
#define MAX_SIZE_LIMIT		10737418240.0	// 10GB
#define MAX_PENDING			10				// Maximum number of pending connections
#define POLL_TIME_OUT		200				// 200ms
#define BUFFER_SIZE			2048			// 2KB
#define SENT_TIMEOUT		60				// 60s
#define MAX_URI_LENGHT		1024			// 1024 characters	
#define MAX_LOOP_COUNT		10				// 10 loops
#define WRITE_END			1
#define READ_END			0

#define DECIMAL				"0123456789"
#define SUFIX_BYTES			"bBkKmMgG"

// Parser check input errors
#define ERR_ARG							"Invalid arguments\n\tUsage: ./webserv [config_file]"
#define ERR_FILE						" is a invalid file\n\tFile must have a name and must be .conf"
#define ERR_OPEN						"Couldn't open file "
#define ERR_INVALID_KEY(token, line)	"Invalid keyword '" + token + "' at line " + line
#define ERR_MISSING_VALUE(token, line)		"Missing value for keyword '" + token + "' at line " + line
#define ERR_MISSING_OPEN_BRACKET(line)	"Missing '{' at line " + line
#define ERR_MANY_VALUES(token, line)	"Too many values for keyword '" + token + "' at line " + line
#define ERR_SEMICOLON(line)				"Missing ';' at line " + line
#define ERR_UNCLOSED_BRACKETS			"Unclosed brackets"
#define ERR_NO_SERVER_CONFIG			"No server configuration found"
#define ERR_BLOCK_START(token, line)	"Invalid block start with '" + token + "' at line " + line + ". Expected 'server' keyword"
#define ERR_SERVER_BLOCK				"Missing '{' after 'server' keyword"
#define ERR_KEYWORD_MISSING(keyword)	"Missing keyword '" + keyword + "' in server block"
#define ERR_FORBIDDEN_KEYWORD(keyword)	"Keyword '" + keyword + "' is not allowed in server block"
#define ERR_DUPLICATE_NAME(name)		"Duplicate server name '" + name + "'. Server names must be unique"
#define ERR_DIRECTORY(path)				"'" + path + "' is not a valid directory"
#define ERR_PAGE_EXT(page)				"'" + page + "' is a invalid file. Pages must be .html"
#define ERR_PAGE_FIND(page)				"Couldn't open page '" + page + "'"
#define ERR_PORT_INPUT(port)			"'"+ port + "' is not a valid port number. Port must be a number between 0 and 65535"
#define ERR_MAX_SIZE_INPUT(size)		"'" + size + "' is not a valid size. Size must be a number positive or a number followed by a sufix (b - B, k - K, m - M, g - G)"
#define ERR_MAX_SIZE_RANGE(size)		"'" + size + "' is not a valid size. The max value allowed is 10G (10737418240 bytes)"
#define ERR_SOCKET(server)				"failed to create network socket for server " + server
#define ERR_SET_SOCKET					"setsockopt() failed: "
#define ERR_GET_ADDR_INFO				"getaddrinfo() failed: "
#define ERR_BIND_SOCKET					"bind() failed: "
#define ERR_LISTEN_SOCKET				"listen() failed: "
#define ERR_POLL_FAIL					"poll() failed"
#define ERR_ACCEPT_SOCKET				"accept() failed"
#define ERR_NO_CGI_EXT					".conf file must have a 'cgi_ext' keyword"
#define ERR_CGI_NO_PYTHON(extension)	"This server doesn't support '" + extension + "' scripts. Only '.py' scripts are supported"
#define ERR_SCRIPT_NAME					"No SCRIPT_FILENAME found in environment"
#define ERR_SCRIPT_NO_EXT(extension)	"No extension found. File must have the extension '" + extension + "'"
#define ERR_SCRIPT_EXT(extension, tmp)	"Invalid extension. File must have the extension '" + extension + "' and not '" + tmp + "'"
#define ERR_SCRIPT_INV(script)			"Couldn't open script '" + script + "'"
#define ERR_INVALID_UPLOAD(uploadTo)	"'" + uploadTo + "' is not a valid directory"				
#define ERR_SCRIPT_OPEN					"open() failed:"
#define ERR_SCRIPT_PIPE					"pipe() failed:"
#define ERR_SCRIPT_FORK					"fork() failed:"
#define ERR_SCRIPT_EXEC					"execve() failed:"

// printInfo messages
#define START_MSG					"Starting Webserv Engine..."
#define END_MSG						"Webserv Engine shutdown complete"
#define SETUP_MSG					"Setting up servers..."
#define SET_SERVER_MSG(host, port)	"Server " + host + ":" + port + " setup complete"
#define LAUNCH_MSG					"Launching servers..."
#define SHUTDOWN_MSG				"Shutting down Webserv Engine..."
#define POLLERR_MSG					"Connection closed. Error: POLLERR"
#define POLLHUP_MSG					"Connection closed. Error: POLLHUP"
#define POLLNVAL_MSG				"Connection closed. Error: POLLNVAL"
#define CLOSE_MSG					"Connection closed"
#define TIMEOUT_MSG					"Connection closed. Timeout"
#define EMPTY_MSG					""

# define BLUE "\033[1;34m"
# define CYAN "\033[1;36m"
# define GREEN "\033[1;32m"
# define GREY "\033[1;30m"
# define MAGENTA "\033[1;35m"
# define RED "\033[1;31m"
# define WHITE "\033[1;37m"
# define YELLOW "\033[1;33m"
# define BLINK "\033[5m"
# define BOLD "\033[1m"
# define STRIKETHROUGH "\033[9m"
# define UNDERLINE "\033[4m"
# define RESET "\033[0m"
# define REVERSE "\033[7m"

#endif