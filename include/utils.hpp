/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsouza-o <nsouza-o@student.42porto.com     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:09:54 by nsouza-o          #+#    #+#             */
/*   Updated: 2024/11/28 16:23:45 by nsouza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <sys/stat.h>
#include <fstream>
#include <algorithm>
#include <vector>

std::vector<std::string> splitServerBlocks(const std::string& content);
std::vector<std::string> splitStr(const std::string& Str, char delimiter);
std::vector<std::string> splitServerStr(const std::string& Str);
std::string intToStr(int i);