// Copyright (c) 2018 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <chem/gauss_data.h>
#include <chem/gaussian.h>
#include <stdutils/stdutils.h>
#include <cstdlib>
#include <fstream>

void Chem::Gaussian::init(std::istream& from, const std::string& key)
{
    using namespace Stdutils;

    // Read input data:

    int nosave_tmp = 1;

    auto pos = find_token(from, key);
    if (pos != -1) {
        get_token_value(from, pos, "version", version, std::string("rung09"));
        get_token_value(from, pos, "jobname", jobname, std::string("gauss"));
        get_token_value(from, pos, "nprocshared", nprocshared, 1);
        get_token_value(from, pos, "nosave", nosave_tmp, nosave_tmp);
        pos = find_token(from, "keywords", pos);
        if (pos != -1) {
            std::getline(from, keywords);
            if (keywords.empty()) { // not entirely safe
                std::getline(from, keywords);
            }
        }
    }

    // Validate input data:

    Assert::dynamic(nosave_tmp == 0 || nosave_tmp == 1, "bad nosave value");
    nosave = false;
    if (nosave_tmp == 1) {
        nosave = true;
    }
    Assert::dynamic(nprocshared >= 1, "bad nprocshared value");
}

void Chem::Gaussian::run(Chem::Molecule& mol) const
{
    write_com(mol); // create Gaussian input file

    bool ok = true;
    std::string cmd = version + " " + jobname;
    if (std::system(cmd.c_str()) != 0) {
        ok = false; // running Gaussian failed
    }

    std::ifstream logfile;
    Stdutils::fopen(logfile, jobname + ".log");
    Chem::Gauss_data data(logfile, out); // get output data

    if (!data.check_termination()) { // Gaussian did not terminate normally
        ok = false;
    }
    if (!data.check_opt_conv()) { // stationary point not found
        ok = false;
    }
    if (ok) {
        Chem::Gauss_coord coord;
        data.get_opt_cart_coord(coord);
        mol.set_xyz(coord.xyz); // update Cartesian coordinates
        mol.elec().set_energy(data.get_scf_zpe_energy()[0]); // update energy
    }
    else { // calculation failed to converge; set energy to zero:
        constexpr double emax = 0.0;
        mol.elec().set_energy(emax);
    }
}

//------------------------------------------------------------------------------

void Chem::Gaussian::write_com(const Chem::Molecule& mol) const
{
    std::ofstream to;
    Stdutils::fopen(to, jobname + ".com");
    to << "%nprocshared=" << nprocshared << '\n'
       << "%chk=" << jobname << ".chk" << '\n';
    if (nosave) {
        to << "%nosave\n";
    }
    to << "# " << keywords << "\n\n"
       << mol.title() << "\n\n"
       << mol.elec().charge() << " " << mol.elec().spin_mult() << '\n';

    Stdutils::Format<double> fix;
    fix.fixed().width(10).precision(6);
    for (std::size_t i = 0; i < mol.num_atoms(); ++i) {
        to << mol.atoms()[i].atomic_symbol << '\t';
        for (Index j = 0; j < mol.get_xyz().cols(); ++j) {
            to << fix(mol.get_xyz()(i, j)) << "  ";
        }
        to << '\n';
    }
    to << '\n';
}

