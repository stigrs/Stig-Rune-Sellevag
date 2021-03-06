// Copyright (c) 2009-2018 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <chem/units.h>
#include <stdexcept>

Chem::Units::Type Chem::Units::lexer(const std::string& unit)
{
    enum Type ans;
    if ((unit == "kJ/mol") || (unit == "kJ mol**-1")) {
        ans = kJ_mol;
    }
    else if ((unit == "kcal/mol") || (unit == "kcal mol**-1")) {
        ans = kcal_mol;
    }
    else if ((unit == "cm**-1") || (unit == "cm^-1") || (unit == "cm-1")) {
        ans = icm;
    }
    else if ((unit == "kelvin") || (unit == "K")) {
        ans = kelvin;
    }
    else if ((unit == "hartree") || (unit == "Eh")) {
        ans = hartree;
    }
    else if ((unit == "hertz") || (unit == "Hertz") || (unit == "s**-1") ||
             (unit == "s^-1") || (unit == "s-1")) {
        ans = hertz;
    }
    else if (unit == "eV") {
        ans = eV;
    }
    else if (unit == "amu") {
        ans = amu;
    }
    else if (unit == "kg") {
        ans = kg;
    }
    else if ((unit == "au") || (unit == "a.u.")) {
        ans = au;
    }
    else {
        throw std::runtime_error("unknown unit: " + unit);
    }
    return ans;
}

void Chem::Units::print(std::ostream& to)
{
    to << "Supported units:\n"
       << " kJ/mol, kJ mol**-1\n"
       << " kcal/mol, kcal mol**-1\n"
       << " cm**-1, cm^-1, cm-1\n"
       << " kelvin, K\n"
       << " hartree, Eh\n"
       << " hertz, Hertz, s**-1, s^-1, s-1\n"
       << " eV\n"
       << " amu\n"
       << " kg\n"
       << " au, a.u.\n";
}

