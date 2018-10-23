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

#ifndef CHEM_VIBRATION_H
#define CHEM_VIBRATION_H

#include <chem/impl/geometry.h>
#include <chem/impl/rotation.h>
#include <numlib/math.h>
#include <numlib/matrix.h>
#include <iostream>
#include <string>

namespace Chem {

namespace Impl {

    // Class for handling molecular vibrations.
    //
    class Vibration {
    public:
        Vibration() = delete;

        Vibration(Geometry& g, Rotation& r) : geom(g), rot(r), freqs{} {}

        Vibration(std::istream& from,
                  const std::string& key,
                  Geometry& g,
                  Rotation& r);

        // Copy semantics:
        Vibration(const Vibration&) = default;
        Vibration& operator=(const Vibration&) = default;

        // Move semantics:
        Vibration(Vibration&&) = default;
        Vibration& operator=(Vibration&&) = default;

        ~Vibration() = default;

        // Get Hessians.
        const auto& hessians() const { return hess; }

        // Get vibrational frequencies.
        const auto& frequencies() const { return freqs; }

        // Calculate zero-point vibrational energy.
        double zero_point_energy() const;

        // Reduced masses for vibrational modes.
        const auto& red_masses() const { return mu_freqs; }

        // Force constants for vibrational modes.
        const auto& force_constants() const { return k_fc; }

        // Print vibrational modes.
        void print(std::ostream& to = std::cout) const;

    private:
        // Calculate mass-weighted Hessians.
        Numlib::Mat<double> mw_hessians() const;

        // Calculate vibrational normal modes.
        void calc_normal_modes();

        // Set up coodinate vectors for translation and rotation about
        // principal axes of inertia.
        void trans_rot_vec(Numlib::Cube<double>& dmat, int& n_tr_rot) const;

        // Transform Cartesian Hessians to internal coordinates.
        void trans_hess_int_coord(Numlib::Cube<double>& dmat,
                                  Numlib::Mat<double>& lmat,
                                  int& n_tr_rot) const;

        // Shuffle n_vib orthogonal vectors to the beginning of D matrix.
        void shuffle(Numlib::Cube<double>& dmat, int n_tr_rot) const;

        // Convert frequencies from atomic units to cm^-1.
        void freqs_unit_conv(Numlib::Vec<double>& vib) const;

        // Print Cartesian frequencies.
        void print_cart_freqs(std::ostream& to) const;

        // Print normal modes.
        void print_normal_modes(std::ostream& to) const;

        Geometry& geom;
        Rotation& rot;

        Numlib::Symm_mat<double, Numlib::lo> hess; // packed Hessians
        Numlib::Vec<double> freqs;                 // vibrational frequencies
        Numlib::Vec<double> mu_freqs; // reduces masses for vibrational modes
        Numlib::Vec<double> k_fc;     // force constants for vibrational modes
        Numlib::Cube<double> l_cart;  // Cartesian displacements
    };

} // namespace Impl

} // namespace Chem

#endif // CHEM_VIBRATION_H