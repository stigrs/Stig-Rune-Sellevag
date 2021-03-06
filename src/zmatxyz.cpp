// Copyright (c) 2017 Stig Rune Sellevag
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4018 4267) // caused by cxxopts.hpp
#endif

#include <chem/molecule.h>
#include <chem/io.h>
#include <stdutils/stdutils.h>
#include <cxxopts.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

// Program for converting between XYZ and Z matrix file formats.
//
int main(int argc, char* argv[])
{
    // clang-format off
    cxxopts::Options options(argv[0], "Convert between XYZ and Z-matrix file formats");
    options.add_options()
        ("h,help", "display help message")
        ("f,file", "input file", cxxopts::value<std::string>())
        ("xyz", "convert to XYZ", cxxopts::value<bool>()->default_value("false"))
        ("zmat", "convert to Z-matrix", cxxopts::value<bool>()->default_value("false"));
    // clang-format on

    auto args = options.parse(argc, argv);

    std::string input_file;

    if (args.count("help")) {
        std::cout << options.help({"", "Group"}) << '\n';
        return 0;
    }
    if (args.count("file")) {
        input_file = args["file"].as<std::string>();
    }
    else {
        std::cerr << options.help({"", "Group"}) << '\n';
        return 1;
    }

    try {
        std::ifstream from;
        Stdutils::fopen(from, input_file);

        Chem::Molecule mol(from);

        std::ofstream to;
        std::string output_file;
        output_file = Stdutils::strip_suffix(input_file, ".inp");

        if (args["xyz"].as<bool>()) {
            mol.geom().load_zmat(from);
            output_file = output_file + ".xyz";
            Stdutils::fopen(to, output_file.c_str());
            Chem::print_xyz_format(to, mol.atoms(), mol.get_xyz(), "");
        }
        else if (args["zmat"].as<bool>()) {
            output_file = output_file + ".zmat";
            Stdutils::fopen(to, output_file.c_str());
            mol.geom().print_zmat(to);
        }
    }
    catch (std::exception& e) {
        std::cerr << "what: " << e.what() << '\n';
        return 1;
    }
}

