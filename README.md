ChemApps - A C++ Chemistry Toolkit
==================================

ChemApps provides a suite of utility tools and programs for thermochemistry
and chemical kinetics.

Features
--------
* Perform rotational analysis of molecules
* Calculation of thermochemical properties for molecules [1]
* Calculation of reduced moment of inertia for torsional modes by using
  the curvilinear scheme [2]
* Calculation of partition function for torsional modes by using the
  CT-Cw scheme [3]
* Calculation of Wigner [4] and Eckart tunneling corrections [5-7]
* Running Mopac 7, Mopac 5.022mn, and Gaussian calculations
* Conversion of Cartesian coordinates to Z matrix
* Conversion of Z matrix to Cartesian coordinates
* Conformer search using an internal coordinate Monte Carlo Multiple 
  Minima (MCMM) technique with uniform usage scheme [8,9]
* Conventional Transition State Theory

Licensing
---------
ChemApps is released under the [MIT](LICENSE) license.

Obtaining the Source Code
-------------------------
The source code can be obtained from

    git clone git@gitlab.com:stigrs/chemapps.git

Requirements
------------
* [CMake](https://cmake.org) 3.4.3
* [Armadillo] (http://arma.sourceforge.net) 7.950.1
* [GSL] (https://github.com/Microsoft/GSL)

Supported Platforms
-------------------
The test suite that exercises ChemApps has been built and passes successfully 
on the following platforms:
* GNU/Linux using GCC 5.4.0
* OS X El Capitan (10.12) using AppleClang X.X.
* Windows 7 using Visual Studio 2017

Usage of Third Party Libraries
------------------------------
This project makes use of the [Catch](https://https://github.com/philsquared/catch) 
testing library and code from the [qcl](https://github.com/ben-albrecht/qcl) 
project. Please see the [ThirdPartyNotices.txt](ThirdPartyNotices.txt) file 
for details regarding the licensing of Catch and qcl.

Installation
------------
The program is installed by executing the command

    mkdir build && cd build
    cmake ..
    make 
    make install

Testing of the program can be done by executing the command

    make test

Notes and References
--------------------
1.  The thermochemical values are computed in accordance with the methods
    implemented in Gaussian (see the Thermochemistry Whitepaper available
    at http://www.gaussian.com)
2.  Pitzer, K. S. J. Chem. Phys. 1946, vol. 14, p. 239.
3.  Chuang, Y. Y.; Truhlar, D. G. J. Chem. Phys. 2000, vol. 112, p. 1221.
4.  Wigner, E. Z. Physik. Chem. (Leipzig), 1932, vol. B19, p. 203.;
5.  Eckart, E. Phys. Rev., 1962, vol. 35, p. 1303.
6.  Brown, R. L. J. Research NIST, 1981, vol. 86, p. 357.
7.  Johnston, H. S.; Heicklen, J. J. Phys. Chem., 1962, vol. 66, p. 532.
8.  Li, Z.; Scheraga, H. A. Proc. Natl. Acad. Sci., 1987, vol. 84, p. 6611.
9.  Chang, G.; Guida, W. C.; Still, C. J. Am. Chem. Soc., 1989, vol. 111,
    p. 4379.

