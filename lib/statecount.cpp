// Copyright (c) 2018 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <chem/statecount.h>
#include <chem/energy_levels.h>
#include <numlib/constants.h>
#include <numlib/math.h>
#include <vector>
#include <cmath>

Numlib::Vec<double> Chem::Statecount::count(const Chem::Molecule& mol,
                                            int ngrains,
                                            double egrain,
                                            bool sum)
{
    Numlib::Vec<double> rot;
    if (mol.tor().tot_minima() > 0) {
        double sigma = mol.tor().symmetry_number();
        double rotc = Numlib::max(mol.tor().constant());
        double barrier = Numlib::max(mol.tor().pot_coeff());
        if (barrier < 0.01) { // free rotor
            rot = free_rotor(sigma, rotc, ngrains, egrain, sum);
        }
        else { // hindered rotor
            rot = hindered_rotor(sigma, rotc, barrier, ngrains, egrain, sum);
        }
    }
    return bswine(mol.vib().frequencies(), ngrains, egrain, sum, rot);
}

Numlib::Vec<double> Chem::Statecount::bswine(const Numlib::Vec<double>& vibr,
                                             int ngrains,
                                             double egrain,
                                             bool sum,
                                             const Numlib::Vec<double>& rot)
{
    Numlib::Vec<double> res(ngrains);
    if (!rot.empty()) { // initialize with rotational states
        res = rot;
    }
    else {
        if (sum) { // count sum of states
            res = Numlib::ones<Numlib::Vec<double>>(ngrains);
        }
        else { // count density of states
            res(0) = 1.0;
        }
    }
    for (auto w : vibr) {
        int wj = Numlib::round<int>(w / egrain);
        for (int i = wj; i < ngrains; ++i) {
            res(i) += res(i - wj);
        }
    }
    if (!sum) {
        res *= 1.0 / egrain;
    }
    return res;
}

Numlib::Vec<double> Chem::Statecount::steinrab(const Numlib::Vec<double>& vibr,
                                               double sigma,
                                               double rotc,
                                               double barrier,
                                               int ngrains,
                                               double egrain,
                                               bool sum)
{
    Numlib::Vec<double> at(ngrains);
    Numlib::Vec<double> tt(ngrains);

    at(0) = 1.0;
    tt(0) = 1.0;

    double emax = ngrains * egrain;

    if (rotc != 0.0) {
        std::vector<double> rj;
        double dd = 0.0;     // degeneracy
        if (barrier > 1.0) { // hindered rotor
            dd = 1.0;
            rj =
                Chem::Energy_levels::hindered_rotor(sigma, rotc, barrier, emax);
        }
        else { // free rotor
            dd = 2.0;
            rj = Chem::Energy_levels::free_rotor(rotc, emax);
        }
        for (std::size_t k = 0; k < rj.size(); ++k) {
            int rjk = Numlib::round<int>(rj[k] / egrain);
            for (int i = rjk; i < ngrains; ++i) {
                at(i) += dd * tt(i - rjk);
            }
        }
        tt = (1.0 / sigma) * at;
        at = tt;
    }
    for (auto w : vibr) {
        auto rj = Chem::Energy_levels::harmonic_oscillator(w, emax);
        for (std::size_t k = 0; k < rj.size(); ++k) {
            int rjk = Numlib::round<int>(rj[k] / egrain);
            for (int i = rjk; i < ngrains; ++i) {
                at(i) += tt(i - rjk);
            }
        }
        tt = at;
    }
    if (sum) {
        for (int i = 1; i < ngrains; ++i) {
            tt(i) += tt(i - 1);
        }
    }
    else {
        tt *= 1.0 / egrain;
    }
    return tt;
}

Numlib::Vec<double> Chem::Statecount::free_rotor(
    double sigma, double rotc, int ngrains, double egrain, bool sum)
{
    Numlib::Vec<double> res(ngrains);

    double f = -0.5;
    if (sum) {
        f = 0.5;
    }
    double qr = std::sqrt(Numlib::Constants::pi) / (sigma * std::sqrt(rotc));
    double gf = std::tgamma(f + 1.0);
    for (int i = 0; i < ngrains; ++i) {
        res(i) = qr * std::pow(i * egrain, f) / gf;
    }
    return res;
}

Numlib::Vec<double> Chem::Statecount::hindered_rotor(double sigma,
                                                     double rotc,
                                                     double barrier,
                                                     int ngrains,
                                                     double egrain,
                                                     bool sum)
{
    using namespace Numlib::Constants;
    using Numlib::comp_ellint_1;
    using Numlib::comp_ellint_2;

    Numlib::Vec<double> res(ngrains);

    int iv0 = Numlib::round<int>(barrier / egrain - 0.5);

    double v0 = barrier;
    double q1f = std::sqrt(pi) / (sigma * std::sqrt(rotc));
    double ei = 0.0;

    if (sum) { // eq. 4.52 in Forst (2003)
        for (int i = 0; i < iv0; ++i) {
            ei = i * egrain;
            res(i) = (4.0 * q1f * std::sqrt(v0) / std::pow(pi, 1.5)) *
                     (comp_ellint_2(ei / v0) -
                      (1.0 - ei / v0) * comp_ellint_1(ei / v0));
        }
        // Ugly hack for E/V0 = 1 in order to avoid NaN:
        ei = iv0 * egrain;
        res(iv0) = (4.0 * q1f * std::sqrt(v0) / std::pow(pi, 1.5));
        for (int i = iv0 + 1; i < ngrains; ++i) {
            ei = i * egrain;
            res(i) = (4.0 * q1f * std::sqrt(ei) / std::pow(pi, 1.5)) *
                     comp_ellint_2(v0 / ei);
        }
    }
    else {
        for (int i = 0; i < iv0; ++i) {
            ei = i * egrain;
            res(i) = (2.0 * q1f / (std::pow(pi, 1.5) * std::sqrt(v0))) *
                     comp_ellint_1(ei / v0);
        }
        for (int i = iv0 + 1; i < ngrains; ++i) {
            ei = i * egrain;
            res(i) = (2.0 * q1f / (std::pow(pi, 1.5) * std::sqrt(ei))) *
                     comp_ellint_1(v0 / ei);
        }
        // Ugly hack for E/V0 = 1 in order to avoid Inf:
        res(iv0) = 0.5 * (res(iv0 + 1) + res(iv0 - 1));
    }
    return res;
}

