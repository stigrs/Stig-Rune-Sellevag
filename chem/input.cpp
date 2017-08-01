/**
   @file input.cpp
   
   This file is part of ChemApps - A C++ Chemistry Toolkit
   
   Copyright (C) 2016-2017  Stig Rune Sellevag
   
   ChemApps is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   ChemApps is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <chem/input.h>
#include <chem/utils.h>
#include <chem/arma_io.h>


std::istream& operator>>(std::istream& from, Input& inp)
{
    switch(inp.type) {
    case Input::t_int:
        inp.read_int(from);
        break;
    case Input::t_long:
        inp.read_long(from);
        break;
    case Input::t_uint:
        inp.read_uint(from);
        break;
    case Input::t_ulint:
        inp.read_ulint(from);
        break;
    case Input::t_double:
        inp.read_double(from);
        break;
    case Input::t_string:
        inp.read_string(from);
        break;
    case Input::t_ivector:
        inp.read_ivector(from);
        break;
    case Input::t_dvector:
        inp.read_dvector(from);
        break;
    case Input::t_noval:
        throw Input_invalid("cannot read data with no type");
    default:
        throw Input_invalid("cannot read data with unknown type");
    }
    inp.state = Input::init;
    return from;
}

std::ostream& operator<<(std::ostream& to, const Input& inp)
{
    if (inp.state == Input::not_init) {
        to << "not initialized";
    }
    else {
        arma::ivec iv;
        arma::vec  dv;

        switch (inp.type) {
        case Input::t_int:
            to << *static_cast<int*>(inp.data);
            break;
        case Input::t_long:
            to << *static_cast<long*>(inp.data);
            break;
        case Input::t_uint:
            to << *static_cast<unsigned*>(inp.data);
            break;
        case Input::t_ulint:
            to << *static_cast<unsigned long*>(inp.data);
            break;
        case Input::t_double:
            to << *static_cast<double*>(inp.data);
            break;
        case Input::t_string:
            to << *static_cast<std::string*>(inp.data);
            break;
        case Input::t_ivector:
            iv = *static_cast<arma::ivec*>(inp.data);
            chem::print_vector(to, iv);
            break;
        case Input::t_dvector:
            dv = *static_cast<arma::vec*>(inp.data);
            chem::print_vector(to, dv);
            break;
        default:
            throw Input_invalid("unknown type; cannot write data");
        }
    }
    return to;
}

//-----------------------------------------------------------------------------

void Input::read_int(std::istream& from)
{
    int& i = *static_cast<int*>(data);

    std::string buf;
    from >> buf;

    i = chem::from_string<int>(buf);
}

void Input::read_long(std::istream& from)
{
    long& i = *static_cast<long*>(data);

    std::string buf;
    from >> buf;

    i = chem::from_string<long>(buf);
}

void Input::read_uint(std::istream& from)
{
    unsigned& i = *static_cast<unsigned*>(data);

    std::string buf;
    from >> buf;

    i = chem::from_string<unsigned>(buf);
}

void Input::read_ulint(std::istream& from)
{
    unsigned long& i = *static_cast<unsigned long*>(data);

    std::string buf;
    from >> buf;

    i = chem::from_string<unsigned long>(buf);
}

void Input::read_double(std::istream& from)
{
    double& d = *static_cast<double*>(data);

    std::string buf;
    from >> buf;

    d = chem::from_string<double>(buf);
}

void Input::read_string(std::istream& from)
{
    std::string& s = *static_cast<std::string*>(data);

    std::string buf;
    from >> buf;

    s = chem::from_string<std::string>(buf);
}

void Input::read_ivector(std::istream& from)
{
    arma::ivec& v = *static_cast<arma::ivec*>(data);
    chem::read_vector(from, v);
}

void Input::read_dvector(std::istream& from)
{
    arma::vec& v = *static_cast<arma::vec*>(data);
    chem::read_vector(from, v);
}
