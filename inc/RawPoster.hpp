/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RawPoster.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 10:25:55 by ndo-vale          #+#    #+#             */
/*   Updated: 2025/03/06 11:06:49 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAW_POSTER_HPP
# define RAW_POSTER_HPP

# include "APostPerformer.hpp"
# include "StateMachine.hpp"
# include <cstdlib> //for strtol
# include <fstream>
# include "utils.hpp"

class RawPoster: public APostPerformer
{
public:
    RawPoster(HttpResponse&  response, std::string saveFileName, size_t bodySize);
    ~RawPoster();

    size_t  post(data_t& data);
private:
    size_t          _bodySize;
    std::ofstream   _saveFile;
};

#endif