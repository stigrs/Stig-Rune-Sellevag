////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017 Stig Rune Sellevag. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CHEM_MOLECULE_H
#define CHEM_MOLECULE_H

#include <chem/impl/elec_state.h>
#include <chem/impl/geometry.h>
#include <chem/impl/rotation.h>
#include <chem/impl/vibration.h>
#include <chem/impl/torsion.h>
#include <chem/traits.h>
#include <iostream>
#include <string>

namespace Chem {

// Class for holding molecule objects.
//
class Molecule {
public:
    Molecule() = delete;

    Molecule(std::istream& from,
             const std::string& key = "Molecule",
             bool verbose = false);

    ~Molecule() = default;

    //
    // Molecular properties:
    //

    // Get information string for molecule.
    std::string info() const { return geom.info(); }

    // Get molecular structure.
    Mol_type structure() const;

    // Get number of atoms.
    auto num_atoms() const { return geom.atoms().size(); }

    // Get atoms in molecule.
    const auto& atoms() const { return geom.atoms(); }

    // Get total molecular mass.
    double tot_mass() const { return geom.tot_mass(); }

    // Get net electronic charge.
    auto net_charge() const { return elec.net_charge(); }

    // Set net electronic charge.
    void set_net_charge(int charge) { elec.set_net_charge(charge); }

    // Get spin multiplicity.
    auto spin_mult() const { return elec.spin_mult(); }

    // Set spin multiplicity.
    void set_spin_mult(int spin) { elec.set_spin_mult(spin); }

    // Get electronic energy.
    auto elec_energy() const { return elec.elec_energy(); }

    // Set electronic energy.
    void set_elec_energy(double energy) { elec.set_elec_energy(energy); }

    // Get degeneracies of spin-orbit states.
    const auto& spin_orbit_degen() const { return elec.spin_orbit_degen(); }

    // Get energies of spin-orbit states.
    const auto& spin_orbit_energy() const { return elec.spin_orbit_energy(); }

    // Get Cartesian coordinates.
    auto& cart_coord() { return geom.cart_coord(); }
    const auto& cart_coord() const { return geom.cart_coord(); }

    void set_cart_coord(const Numlib::Mat<double>& x)
    {
        geom.set_cart_coord(x);
    }

    // Get internal coordinates.
    auto& int_coord() { return geom.int_coord(); }
    const auto& int_coord() const { return geom.int_coord(); }

    // Get rotational symmetry number.
    auto rot_sigma() const { return rot.rot_sigma(); }

    // Get rotational constants.
    auto rot_constants() const { return rot.constants(); }

    // Get rotational symmetry.
    auto rot_symmetry() const { return rot.symmetry(); }

    // Get principal moments.
    auto principal_moments() const { return rot.principal_moments(); }

    // Get principal axes.
    auto principal_axes() const { return rot.principal_axes(); }

    // Get Hessians.
    const auto& hessians() const { return vib.hessians(); }

    // Zero-point vibrational energy.
    double zero_point_energy() const { return vib.zero_point_energy(); }

    // Vibrational frequencies.
    const auto& frequencies() const { return vib.frequencies(); }

    // Reduced masses of vibrational modes.
    const auto& vib_red_masses() const { return vib.red_masses(); }

    // Force constants of vibrational modes.
    const auto& vib_force_constants() const { return vib.force_constants(); }

    // Total number of torsional minima.
    int tot_tor_minima() const { return tor.tot_minima(); }

    // Effective torsional symmetry number.
    double tor_symmetry_number() const { return tor.symmetry_number(); }

    // Reduced moment of inertia for torsional mode.
    double tor_red_moment() { return tor.red_moment(); }

    // Effective moment of inertia for torsional mode.
    double tor_eff_moment() const { return tor.eff_moment(); }

    // Rotational constant for torsional mode.
    auto tor_constant() const { return tor.constant(); }

    // Potential coefficients for torsional mode.
    const auto& tor_pot_coeff() const { return tor.pot_coeff(); }

    // Torsional frequencies.
    const auto& tor_frequencies() const { return tor.frequencies(); }

    //
    // Perform analysis of molecular properties.
    //

    // Rotational analysis.
    void rot_analysis(std::ostream& to = std::cout) const
    {
        return rot.analysis(to);
    }

    // Vibrational analysis.
    void vib_analysis(std::ostream& to = std::cout) const
    {
        return vib.analysis(to);
    }

    // Torsional analysis.
    void tor_analysis(std::ostream& to = std::cout) const
    {
        return tor.analysis(to);
    }

private:
    Impl::Elec_state elec; // molecular electronic states
    Impl::Geometry geom;   // molecular geometry
    Impl::Rotation rot;    // molecular rotations
    Impl::Vibration vib;   // molecular vibrations
    Impl::Torsion tor;     // internal torsional modes
};

inline Mol_type Molecule::structure() const
{
    Mol_type res;
    if (num_atoms() == 1) {
        res = atom;
    }
    else if (rot_constants().size() == 1) {
        res = linear;
    }
    else {
        res = nonlinear;
    }
    return res;
}

} // namespace Chem

#endif // CHEM_MOLECULE_H

