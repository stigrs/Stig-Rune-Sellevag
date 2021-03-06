// Copyright (c) 2013-2018 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef CHEM_TROE_H
#define CHEM_TROE_H

#include <chem/traits.h>
#include <chem/molecule.h>
#include <cmath>
#include <iostream>
#include <string>

namespace Chem {

// Class to perform Troe factorization of strong-collision low-pressure
// limiting rate coefficients for dissociation reactions as presented in
// the papers:
//
//   Troe, J. J. Chem. Phys., 1977, vol. 66, pp. 4745-4757.
//   Troe, J. J. Chem. Phys., 1977, vol. 66, pp. 4758-4775.
//
// The formalism has been extended to include calculation of reduced collision
// integrals using Eq. A4.10 given in
//
//   Forst, W. Unimolecular Reactions; Cambridge University Press, 2003.
//
// Note: Only neutral species can be investigated in this version.
//
class Troe {
public:
    Troe(std::istream& from, Molecule& mol_);

    // Calculate energy dependence factor.
    //
    // Eq. 9.10 in Troe, J. J. Chem. Phys., 1977, vol. 66, pp. 4758--4775.
    //
    double f_energy(const double temp) const;

    // Calculate anharmonicity factor for m Morse oscillators with
    // dissociation energies equal to the barrier height of reaction.
    //
    // Eq. 5.4 in Troe, J. J. Chem. Phys., 1977, vol. 66, pp. 4758--4775.
    //
    double f_anharm() const;

    // Calculate rotational factor.
    //
    // Eq. 7.23 (linear molecule, Case II potential), Eq. 7.24 (nonlinear
    // molecule, Case I potential), Eq. 7.26 (linear molecule, Case II
    // potential), and Eq. 7.27 (nonlinear molecule, Case I potential) in
    // Troe, J. J. Chem. Phys., 1977, vol. 66, pp. 4758--4775.
    //
    double f_rotation(const double temp) const;

    // Calculate free internal rotation factor.
    //
    // Eq. 9.15 in Troe, J. J. Chem. Phys., 1977, vol. 66, pp. 4758--4775.
    //
    double f_free_rotor(const double temp) const;

    // Calculate hindered internal rotation factor.
    //
    // Eq. 9.16 in Troe, J. J. Chem. Phys., 1977, vol. 66, pp. 4758--4775.
    // Hindered internal rotation with E0/V0 <= 3 cannot be treated yet.
    //
    double f_hind_rotor(const double temp) const;

    // Get reaction barrier.
    double get_energy_barrier() const { return en_barrier; }

private:
    Molecule& mol;
    Pot_type pot_type; // potential type (type 1 or type 2)
    double en_barrier; // energy barrier for the reaction
    double imom_ratio; // moment of inertia ratio
    int n_free_rot;    // number of free rotors
    int n_morse_osc;   // number of Morse oscillators

    double zpe; // zero-point vibrational energy
};

inline double Troe::f_anharm() const
{
    auto s = mol.vib().frequencies().size();

    return std::pow((s - 1.0) / (s - 1.5), n_morse_osc);
}

} // namespace Chem

#endif // CHEM_TROE_H

