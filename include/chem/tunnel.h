// Copyright (c) 2017 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef CHEM_TUNNEL_H
#define CHEM_TUNNEL_H

#include <numlib/constants.h>
#include <stdutils/stdutils.h>
#include <cmath>
#include <iostream>
#include <string>

namespace Chem {

// Class for computing quantum tunneling corrections.
//
class Tunnel {
public:
    Tunnel() { method = None; }
    Tunnel(std::istream& from, const std::string& key = "Tunnel");

    // Get tunneling correction method.
    std::string get_method() const;

    // Calculate Wigner tunneling correction.
    double wigner(double temp = 298.15) const;

    // Calculate Eckart tunneling correction for an unsymmetrical barrier.
    double eckart(double temp = 298.15) const;

    // Calculate tunneling correction factor.
    double factor(double temp = 298.15) const;

private:
    enum Method_t { None, Wigner, Eckart };

    Method_t method = None; // tunneling correction method
    double freq_im;         // imaginary frequency
    double en_barrier;      // potential barrier height
    double en_rxn;          // energy of reaction
};

inline double Tunnel::wigner(double temp) const
{
    // Wigner, E. Z. Physik. Chem. (Leipzig), 1932, vol. B19, p. 203.

    using namespace Numlib::Constants;
    Assert::dynamic<Assert::level(2)>(temp > 0.0, "bad temperature");
    double factor = h * std::abs(freq_im) * 100.0 * c_0 / (k * temp);
    return 1.0 + factor * factor / 24.0;
}

inline double Tunnel::factor(double temp) const
{
    switch (method) {
    case Wigner:
        return wigner(temp);
    case Eckart:
        return eckart(temp);
    case None:
    default:
        return 1.0;
    }
}

inline std::string Tunnel::get_method() const
{
    switch (method) {
    case Wigner:
        return "Wigner";
    case Eckart:
        return "Eckart";
    case None:
    default:
        return "None";
    }
}

} // namespace Chem

#endif // CHEM_TUNNEL_H

