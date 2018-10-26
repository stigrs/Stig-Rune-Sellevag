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

#include <chem/mopac.h>
#include <numlib/constants.h>
#include <stdutils/stdutils.h>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <limits>
#include <sstream>

void Chem::Mopac::init(std::istream& from, const std::string& key)
{
    // Read input data:

    using namespace Stdutils;

    auto pos = find_token(from, key);
    if (pos != -1) {
        get_token_value(from, pos, "version", version,
                        std::string("mopac5022mn"));
        get_token_value(from, pos, "jobname", jobname, std::string("mopac"));
        get_token_value(from, pos, "opt_geom", opt_geom, 1);
        pos = find_token(from, "keywords", pos);
        if (pos != -1) {
            std::string line;
            std::getline(from, line);
            if (line.empty()) { // not entirely safe
                std::getline(from, line);
            }
            keywords = trim(line, " ");
        }
    }
}

void Chem::Mopac::run(Chem::Molecule& mol) const
{
    write_dat(mol); // create Mopac input file

    bool ok = true;
    std::string cmd = version + " " + jobname + ".dat";
    if (std::system(cmd.c_str()) != 0) {
        ok = false; // running Mopac failed
    }

    if (!check_convergence()) {
        ok = false; // optimization failed
    }
    if (ok) {
        mol.set_elec_energy(get_heat_of_formation()); // update energy

        Numlib::Mat<double> xyz = mol.cart_coord();
        get_xyz(xyz);
        mol.cart_coord() = xyz; // update Cartesian coordinates
    }
    else { // calculation failed to converge; set energy to infinity
        constexpr double emax = std::numeric_limits<double>::max();
        mol.set_elec_energy(emax);
    }
}

void Chem::Mopac::write_dat(const Chem::Molecule& mol) const
{
    std::ofstream to;
    Stdutils::fopen(to, jobname + ".dat");
    to << keywords << '\n' << mol.info() << "\n\n";
    write_xyz(to, mol);
}

void Chem::Mopac::write_xyz(std::ostream& to, const Chem::Molecule& mol) const
{
    Stdutils::Format<double> fix;
    fix.fixed().width(10).precision(6);

    for (std::size_t i = 0; i < mol.num_atoms(); ++i) {
        to << mol.atoms()[i].atomic_symbol << '\t';
        for (Index j = 0; j < mol.cart_coord().cols(); ++j) {
            to << fix(mol.cart_coord()(i, j)) << " " << opt_geom << " ";
        }
        to << '\n';
    }
}

bool Chem::Mopac::check_convergence() const
{
    bool converged = false;

    std::ifstream from;
    Stdutils::fopen(from, jobname + ".out");

    std::string buf;
    while (std::getline(from, buf)) {
        std::string pattern = "SCF FIELD WAS ACHIEVED";
        if (buf.find(pattern) != std::string::npos) {
            converged = true;
            break;
        }
    }
    return converged;
}

double Chem::Mopac::get_heat_of_formation() const
{
    double heat = 0.0;
    bool found = false;

    std::ifstream from;
    Stdutils::fopen(from, jobname + ".out");

    std::string buf;
    while (std::getline(from, buf)) {
        std::string pattern = "SCF FIELD WAS ACHIEVED"; // is this fail-safe?
        if (buf.find(pattern) != std::string::npos) {
            while (std::getline(from, buf)) {
                pattern = "FINAL HEAT OF FORMATION = ";
                if (buf.find(pattern) != std::string::npos) {
                    std::istringstream iss(buf);
                    for (int i = 0; i < 5; ++i) { // ignore words in pattern
                        iss >> buf;
                    }
                    iss >> heat;
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("final heat of formation not found");
    }
    else {
        return heat * Numlib::Constants::cal_to_J;
    }
}

void Chem::Mopac::get_xyz(Numlib::Mat<double>& xyz) const
{
    // Note: xyz must have the correct size on input, no resizing is done.

    bool found = false;

    std::ifstream from;
    Stdutils::fopen(from, jobname + ".out");

    std::string buf;
    while (std::getline(from, buf)) {
        std::string pattern = "SCF FIELD WAS ACHIEVED"; // is this fail-safe?
        if (buf.find(pattern) != std::string::npos) {
            while (std::getline(from, buf)) {
                pattern = "CARTESIAN COORDINATES";
                if (buf.find(pattern) != std::string::npos) {
                    for (int i = 0; i < 3; ++i) { // ignore three lines
                        std::getline(from, buf);
                    }
                    int center;
                    std::string atom;
                    double x;
                    double y;
                    double z;
                    for (Index i = 0; i < xyz.rows(); ++i) {
                        std::getline(from, buf);
                        std::istringstream iss(buf);
                        iss >> center >> atom >> x >> y >> z;
                        xyz(i, 0) = x;
                        xyz(i, 1) = y;
                        xyz(i, 2) = z;
                    }
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("optimized Cartesian coordinates not found");
    }
}

