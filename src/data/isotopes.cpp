/*
    *** Sears '91 Neutron Scattering Length Data
    *** src/data/isotopes.cpp
    Copyright T. Youngs 2012-2020

    This file is part of Dissolve.

    Dissolve is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Dissolve is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Dissolve.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "data/isotopes.h"
#include "base/messenger.h"

// Static Singletons
Array<List<Isotope>> Isotopes::isotopesByElementPrivate_;

/*
 * Isotopic Neutron Scattering Data
 */

Isotope::Isotope(int z, int A, std::string_view spin, double mass, double bc, double bi, double sc, double si, double totalxs,
                 double absxs)
    : ElementReference(z), ListItem<Isotope>()
{
    // Set the isotope's data
    A_ = A;
    spin_ = spin;
    mass_ = mass;
    boundCoherent_ = bc;
    boundIncoherent_ = bi;
    boundCoherentXS_ = sc;
    boundIncoherentXS_ = si;
    totalXS_ = totalxs;
    absorptionXS_ = absxs;

    // Add this isotope to its parent element's list
    Isotopes::registerIsotope(this, z);
}

Isotope &Isotope::operator=(const Isotope &source)
{
    A_ = source.A_;
    spin_ = source.spin_;
    mass_ = source.mass_;
    boundCoherent_ = source.boundCoherent_;
    ;
    boundIncoherent_ = source.boundIncoherent_;
    boundCoherentXS_ = source.boundCoherentXS_;
    boundIncoherentXS_ = source.boundIncoherentXS_;
    totalXS_ = source.totalXS_;
    absorptionXS_ = source.absorptionXS_;

    return (*this);
}

// Return index of isotope in it's Element parent's list
int Isotope::index() const { return Isotopes::isotopes(element().Z()).indexOf(this); }

// Return mass number (A) of Isotope
int Isotope::A() const { return A_; }

// Return isotope mass (given C = 12)
double Isotope::mass() const { return mass_; }

// Return bound coherent scattering length (fm)
double Isotope::boundCoherent() const { return boundCoherent_; }

// Return bound incoherent scattering length (fm)
double Isotope::boundIncoherent() const { return boundIncoherent_; }

// Return bound coherent scattering cross section (barn)
double Isotope::boundCoherentXS() const { return boundCoherentXS_; }

// Return bound incoherent scattering cross section (barn)
double Isotope::boundIncoherentXS() const { return boundIncoherentXS_; }

// Return total bound scattering cross section (barn)
double Isotope::totalXS() const { return totalXS_; }

// Return absorption cross section for thermal (2200 m/s) neutron (barn)
double Isotope::absorptionXS() const { return absorptionXS_; }

/*
 * Isotopes (Sears '91) Helper Class
 */

// Return isotope data, grouped by element
List<Isotope> &Isotopes::isotopesByElement(int Z)
{
    // Has the master array been initialised yet? If not, do it now, before the Sears data is constructed
    if (isotopesByElementPrivate_.nItems() == 0)
        Elements::createElementListArray<Isotope>(isotopesByElementPrivate_);

    /*
     * Neutron Scattering Lengths and Cross Sections
     *
     * Varley F. Sears
     * AECL Research, Chalk River Laboratories
     * Chalk River, Ontario, Canada K0J 1J0
     * 1991 November 11
     *
     * DRAFT
     *
     *	Symbol  Unit    Quantity
     *	--------------------------------------------------------------
     *	Z		atomic number
     *	A		mass number
     *	I(p)		spin (parity) of the nuclear ground state
     *	Atwt		Atomic weight (C=12)
     *	bc	fm	bound coherent scattering length
     *	bi	fm	bound incoherent scattering length
     *	sc	barn[1]	bound coherent scattering cross section
     *	si	barn	bound incoherent scattering cross section
     *	ss	barn	total bound scattering cross section
     *	sa	barn	absorption cross section for 2200 m/s neutrons[2]
     *
     *	[1] 1 barn = 100 fm**2
     *	[2] E = 25.30 meV, k = 3.494 Angstroms**-1, lambda = 1.798 Angstroms
     */

    static Isotope sears91Data[] = {
        //	Z	El	A	I(p)		Mass		bc	bi	sc	si	ss	sa
        {ELEMENT_XX, 0, "", 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {ELEMENT_H, 0, "", 1.00797, -3.739, 0, 1.7568, 80.26, 82.02, 0.3326},
        {ELEMENT_H, 1, "1/2(+)", 1.00783, -3.7406, 25.274, 1.7583, 80.27, 82.03, 0.3326},
        {ELEMENT_H, 2, "1(+)", 2.014, 6.671, 4.04, 5.592, 2.05, 7.64, 0.000519},
        {ELEMENT_H, 3, "1/2(+)", 3.01605, 4.792, -1.04, 2.89, 0.14, 3.03, 0},
        {ELEMENT_HE, 0, "", 4.0026, 3.26, 0, 1.34, 0, 1.34, 0.00747},
        {ELEMENT_HE, 3, "1/2(+)", 3.01603, 5.74, -2.5, 4.42, 1.6, 6, 5333},
        {ELEMENT_HE, 4, "0(+)", 4.0026, 3.26, 0, 1.34, 0, 1.34, 0},
        {ELEMENT_LI, 0, "", 6.9409, -1.9, 0, 0.454, 0.92, 1.37, 70.5},
        {ELEMENT_LI, 6, "1(+)", 6.01512, 2, -1.89, 0.51, 0.46, 0.97, 940},
        {ELEMENT_LI, 7, "3/2(-)", 7.016, -2.22, -2.49, 0.619, 0.78, 1.4, 0.0454},
        {ELEMENT_BE, 0, "3/2(-)", 9.01218, 7.79, 0.12, 7.63, 0.0018, 7.63, 0.0076},
        {ELEMENT_B, 0, "", 10.81003, 5.3, 0, 3.54, 1.7, 5.24, 767},
        {ELEMENT_B, 10, "3(+)", 10.01294, -0.1, -4.7, 0.144, 3, 3.1, 3835},
        {ELEMENT_B, 11, "3/2(-)", 11.00931, 6.65, -1.3, 5.56, 0.21, 5.77, 0.0055},
        {ELEMENT_C, 0, "", 12.0111, 6.646, 0, 5.55, 0.001, 5.551, 0.0035},
        {ELEMENT_C, 12, "0(+)", 12, 6.6511, 0, 5.559, 0, 5.559, 0.00353},
        {ELEMENT_C, 13, "1/2(-)", 13.00336, 6.19, -0.52, 4.81, 0.034, 4.84, 0.00137},
        {ELEMENT_N, 0, "", 14.0067, 9.36, 0, 11.01, 0.5, 11.51, 1.9},
        {ELEMENT_N, 14, "1(+)", 14.00307, 9.37, 2, 11.03, 0.5, 11.53, 1.91},
        {ELEMENT_N, 15, "1/2(-)", 15.00011, 6.44, -0.02, 5.21, 0.0001, 5.21, 2.4E-05},
        {ELEMENT_O, 0, "", 15.9994, 5.803, 0, 4.232, 0, 4.232, 0.00019},
        {ELEMENT_O, 16, "0(+)", 15.99492, 5.803, 0, 4.232, 0, 4.232, 0.0001},
        {ELEMENT_O, 17, "5/2(+)", 16.99913, 5.78, 0.18, 4.2, 0.004, 4.2, 0.236},
        {ELEMENT_O, 18, "0(+)", 17.99916, 5.84, 0, 4.29, 0, 4.29, 0.00016},
        {ELEMENT_F, 0, "1/2(+)", 18.9984, 5.654, -0.082, 4.017, 0.0008, 4.018, 0.0096},
        {ELEMENT_NE, 0, "", 20.179, 4.566, 0, 2.62, 0.008, 2.628, 0.039},
        {ELEMENT_NE, 20, "0(+)", 19.99244, 4.631, 0, 2.695, 0, 2.695, 0.036},
        {ELEMENT_NE, 21, "3/2(+)", 20.99384, 6.66, 0, 5.6, 0.05, 5.7, 0.67},
        {ELEMENT_NE, 22, "0(+)", 21.99138, 3.87, 0, 1.88, 0, 1.88, 0.046},
        {ELEMENT_NA, 0, "3/2(+)", 22.98977, 3.58, 3.65, 1.611, 1.674, 3.285, 0.53},
        {ELEMENT_MG, 0, "", 24.305, 5.375, 0, 3.631, 0.08, 3.71, 0.063},
        {ELEMENT_MG, 24, "0(+)", 23.98504, 5.66, 0, 4.03, 0, 4.03, 0.05},
        {ELEMENT_MG, 25, "5/2(+)", 24.98584, 3.62, 1.48, 1.65, 0.28, 1.93, 0.19},
        {ELEMENT_MG, 26, "0(+)", 25.98259, 4.89, 0, 3, 0, 3, 0.0382},
        {ELEMENT_AL, 0, "5/2(+)", 26.98154, 3.449, 0.256, 1.495, 0.0082, 1.503, 0.231},
        {ELEMENT_SI, 0, "", 28.0855, 4.1534, 0, 2.1678, 0.003, 2.171, 0.171},
        {ELEMENT_SI, 28, "0(+)", 27.97693, 4.111, 0, 2.124, 0, 2.124, 0.177},
        {ELEMENT_SI, 29, "1/2(+)", 28.9765, 4.7, 0.09, 2.78, 0.001, 2.78, 0.101},
        {ELEMENT_SI, 30, "0(+)", 29.97377, 4.58, 0, 2.64, 0, 2.64, 0.107},
        {ELEMENT_P, 0, "1/2(+)", 30.97376, 5.13, 0.2, 3.307, 0.005, 3.312, 0.172},
        {ELEMENT_S, 0, "", 32.066, 2.847, 0, 1.0186, 0.007, 1.026, 0.53},
        {ELEMENT_S, 32, "0(+)", 31.97207, 2.804, 0, 0.988, 0, 0.988, 0.54},
        {ELEMENT_S, 33, "3/2(+)", 32.97146, 4.74, 1.5, 2.8, 0.3, 3.1, 0.54},
        {ELEMENT_S, 34, "0(+)", 33.96787, 3.48, 0, 1.52, 0, 1.52, 0.227},
        {ELEMENT_S, 36, "0(+)", 35.96708, 3, 0, 1.1, 0, 1.1, 0.15},
        {ELEMENT_CL, 0, "", 35.453, 9.577, 0, 11.526, 5.3, 16.8, 33.5},
        {ELEMENT_CL, 35, "3/2(+)", 34.96885, 11.65, 6.1, 17.06, 4.7, 21.8, 44.1},
        {ELEMENT_CL, 37, "3/2(+)", 36.9659, 3.08, 0.1, 1.19, 0.001, 1.19, 0.433},
        {ELEMENT_AR, 0, "", 39.948, 1.909, 0, 0.458, 0.225, 0.683, 0.675},
        {ELEMENT_AR, 36, "0(+)", 35.96755, 24.9, 0, 77.9, 0, 77.9, 5.2},
        {ELEMENT_AR, 38, "0(+)", 37.96273, 3.5, 0, 1.5, 0, 1.5, 0.8},
        {ELEMENT_AR, 40, "0(+)", 39.96238, 1.83, 0, 0.421, 0, 0.421, 0.66},
        {ELEMENT_K, 0, "", 39.0983, 3.67, 0, 1.69, 0.27, 1.96, 2.1},
        {ELEMENT_K, 39, "3/2(+)", 38.96371, 3.74, 1.4, 1.76, 0.25, 2.01, 2.1},
        {ELEMENT_K, 40, "4(-)", 39.974, 3, 0, 1.1, 0.5, 1.6, 35},
        {ELEMENT_K, 41, "3/2(+)", 40.96183, 2.69, 1.5, 0.91, 0.3, 1.2, 1.46},
        {ELEMENT_CA, 0, "", 40.078, 4.76, 0, 2.85, 0.04, 2.89, 0.43},
        {ELEMENT_CA, 40, "0(+)", 39.96259, 4.85, 0, 2.96, 0, 2.96, 0.41},
        {ELEMENT_CA, 42, "0(+)", 41.95862, 3.15, 0, 1.25, 0, 1.25, 0.68},
        {ELEMENT_CA, 43, "7/2(-)", 42.95877, 0.2, 0, 0.005, 0.5, 0.5, 6.2},
        {ELEMENT_CA, 44, "0(+)", 43.95548, 1.8, 0, 0.41, 0, 0.41, 0.88},
        {ELEMENT_CA, 46, "0(+)", 45.95369, 2.55, 0, 0.82, 0, 0.82, 0.74},
        {ELEMENT_CA, 48, "0(+)", 47.95253, 1.5, 0, 0.28, 0, 0.28, 1.09},
        {ELEMENT_SC, 0, "7/2(-)", 44.95591, 12.29, -6, 19, 4.5, 23.5, 27.5},
        {ELEMENT_TI, 0, "", 47.88, -3.438, 0, 1.485, 2.87, 4.35, 6.09},
        {ELEMENT_TI, 46, "0(+)", 45.95263, 4.93, 0, 3.05, 0, 3.05, 0.59},
        {ELEMENT_TI, 47, "5/2(-)", 46.95176, 3.63, -3.5, 1.66, 1.5, 3.2, 1.7},
        {ELEMENT_TI, 48, "0(+)", 47.94795, -6.08, 0, 4.65, 0, 4.65, 7.84},
        {ELEMENT_TI, 49, "7/2(-)", 48.94787, 1.04, 5.1, 0.14, 3.3, 3.4, 2.2},
        {ELEMENT_TI, 50, "0(+)", 49.94479, 6.18, 0, 4.8, 0, 4.8, 0.179},
        {ELEMENT_V, 0, "", 50.9415, -0.3824, 0, 0.0184, 5.08, 5.1, 5.08},
        {ELEMENT_V, 50, "6(+)", 49.94716, 7.6, 0, 7.3, 0.5, 7.8, 60},
        {ELEMENT_V, 51, "7/2(-)", 50.94396, -0.402, 6.35, 0.0203, 5.07, 5.09, 4.9},
        {ELEMENT_CR, 0, "", 51.996, 3.635, 0, 1.66, 1.83, 3.49, 3.05},
        {ELEMENT_CR, 50, "0(+)", 49.94605, -4.5, 0, 2.54, 0, 2.54, 15.8},
        {ELEMENT_CR, 52, "0(+)", 51.94051, 4.92, 0, 3.042, 0, 3.042, 0.76},
        {ELEMENT_CR, 53, "3/2(-)", 52.94065, -4.2, 6.87, 2.22, 5.93, 8.15, 18.1},
        {ELEMENT_CR, 54, "0(+)", 53.93888, 4.55, 0, 2.6, 0, 2.6, 0.36},
        {ELEMENT_MN, 0, "5/2(-)", 54.938, -3.73, 1.79, 1.75, 0.4, 2.15, 13.3},
        {ELEMENT_FE, 0, "", 55.847, 9.54, 0, 11.44, 0.38, 11.82, 2.56},
        {ELEMENT_FE, 54, "0(+)", 53.93961, 4.2, 0, 2.2, 0, 2.2, 2.25},
        {ELEMENT_FE, 56, "0(+)", 55.93494, 10.03, 0, 12.64, 0, 12.64, 2.59},
        {ELEMENT_FE, 57, "1/2(-)", 56.9354, 2.3, 0, 0.66, 0.3, 1, 2.48},
        {ELEMENT_FE, 58, "0(+)", 57.93328, 15, 0, 28, 0, 28, 1.28},
        {ELEMENT_CO, 0, "7/2(-)", 58.9332, 2.78, -6.2, 0.97, 4.8, 5.8, 37.18},
        {ELEMENT_NI, 0, "", 58.67, 10.3, 0, 13.3, 5.2, 18.5, 4.49},
        {ELEMENT_NI, 58, "0(+)", 57.93535, 14.4, 0, 26.1, 0, 26.1, 4.6},
        {ELEMENT_NI, 60, "0(+)", 59.93079, 2.8, 0, 0.99, 0, 0.99, 2.9},
        {ELEMENT_NI, 61, "3/2(-)", 60.93106, 7.6, 3.9, 7.26, 1.9, 9.2, 2.5},
        {ELEMENT_NI, 62, "0(+)", 61.92835, -8.7, 0, 9.5, 0, 9.5, 14.5},
        {ELEMENT_NI, 64, "0(+)", 63.92797, -0.38, 0, 0.018, 0, 0.018, 1.52},
        {ELEMENT_CU, 0, "", 63.546, 7.718, 0, 7.485, 0.55, 8.03, 3.78},
        {ELEMENT_CU, 63, "3/2(-)", 62.9296, 6.43, 0.22, 5.2, 0.006, 5.2, 4.5},
        {ELEMENT_CU, 65, "3/2(-)", 64.92779, 10.61, 1.79, 14.1, 0.4, 14.5, 2.17},
        {ELEMENT_ZN, 0, "", 65.39, 5.68, 0, 4.054, 0.077, 4.131, 1.11},
        {ELEMENT_ZN, 64, "0(+)", 63.92915, 5.22, 0, 3.42, 0, 3.42, 0.93},
        {ELEMENT_ZN, 66, "0(+)", 65.92603, 5.97, 0, 4.48, 0, 4.48, 0.62},
        {ELEMENT_ZN, 67, "5/2(-)", 66.92713, 7.56, -1.5, 7.18, 0.28, 7.46, 6.8},
        {ELEMENT_ZN, 68, "0(+)", 67.92485, 6.03, 0, 4.57, 0, 4.57, 1.1},
        {ELEMENT_ZN, 70, "0(+)", 69.92533, 6, 0, 4.5, 0, 4.5, 0.092},
        {ELEMENT_GA, 0, "", 69.723, 7.288, 0, 6.675, 0.16, 6.83, 2.75},
        {ELEMENT_GA, 69, "3/2(-)", 68.92558, 7.88, -0.85, 7.8, 0.091, 7.89, 2.18},
        {ELEMENT_GA, 71, "3/2(-)", 70.9247, 6.4, -0.82, 5.15, 0.084, 5.23, 3.61},
        {ELEMENT_GE, 0, "", 72.59, 8.185, 0, 8.42, 0.18, 8.6, 2.2},
        {ELEMENT_GE, 70, "0(+)", 69.92425, 10, 0, 12.6, 0, 12.6, 3},
        {ELEMENT_GE, 72, "0(+)", 71.92208, 8.51, 0, 9.1, 0, 9.1, 0.8},
        {ELEMENT_GE, 73, "9/2(+)", 72.92346, 5.02, 3.4, 3.17, 1.5, 4.7, 15.1},
        {ELEMENT_GE, 74, "0(+)", 73.92118, 7.58, 0, 7.2, 0, 7.2, 0.4},
        {ELEMENT_GE, 76, "0(+)", 75.9214, 8.2, 0, 8, 0, 8, 0.16},
        {ELEMENT_AS, 0, "3/2(-)", 74.92159, 6.58, -0.69, 5.44, 0.06, 5.5, 4.5},
        {ELEMENT_SE, 0, "", 78.96, 7.97, 0, 7.98, 0.32, 8.3, 11.7},
        {ELEMENT_SE, 74, "0(+)", 73.92248, 0.8, 0, 0.1, 0, 0.1, 51.8},
        {ELEMENT_SE, 76, "0(+)", 75.91921, 12.2, 0, 18.7, 0, 18.7, 85},
        {ELEMENT_SE, 77, "1/2(-)", 76.91991, 8.25, 0, 8.6, 0.05, 8.65, 42},
        {ELEMENT_SE, 78, "0(+)", 77.9173, 8.24, 0, 8.5, 0, 8.5, 0.43},
        {ELEMENT_SE, 80, "0(+)", 79.91652, 7.48, 0, 7.03, 0, 7.03, 0.61},
        {ELEMENT_SE, 82, "0(+)", 81.9167, 6.34, 0, 5.05, 0, 5.05, 0.044},
        {ELEMENT_BR, 0, "", 79.904, 6.795, 0, 5.8, 0.1, 5.9, 6.9},
        {ELEMENT_BR, 79, "3/2(-)", 78.91834, 6.8, -1.1, 5.81, 0.15, 5.96, 11},
        {ELEMENT_BR, 81, "3/2(-)", 80.91629, 6.79, 0.6, 5.79, 0.05, 5.84, 2.7},
        {ELEMENT_KR, 0, "", 83.8, 7.81, 0, 7.67, 0.01, 7.68, 25},
        {ELEMENT_KR, 78, "0(+)", 77.9204, 0, 0, 0, 0, 0, 6.4},
        {ELEMENT_KR, 80, "0(+)", 79.91638, 0, 0, 0, 0, 0, 11.8},
        {ELEMENT_KR, 82, "0(+)", 81.91348, 0, 0, 0, 0, 0, 29},
        {ELEMENT_KR, 83, "9/2(+)", 82.91414, 0, 0, 0, 0, 0, 185},
        {ELEMENT_KR, 84, "0(+)", 83.91151, 0, 0, 0, 0, 0, 0.113},
        {ELEMENT_KR, 86, "0(+)", 85.91062, 0, 0, 0, 0, 0, 0.003},
        {ELEMENT_RB, 0, "", 85.4678, 7.09, 0, 6.32, 0.5, 6.8, 0.38},
        {ELEMENT_RB, 85, "5/2(-)", 84.91179, 7.03, 0, 6.2, 0.5, 6.7, 0.48},
        {ELEMENT_RB, 87, "3/2(-)", 86.90919, 7.23, 0, 6.6, 0.5, 7.1, 0.12},
        {ELEMENT_SR, 0, "", 87.62, 7.02, 0, 6.19, 0.06, 6.25, 1.28},
        {ELEMENT_SR, 84, "0(+)", 83.91343, 7, 0, 6, 0, 6, 0.87},
        {ELEMENT_SR, 86, "0(+)", 85.90927, 5.67, 0, 4.04, 0, 4.04, 1.04},
        {ELEMENT_SR, 87, "9/2(+)", 86.90888, 7.4, 0, 6.88, 0.5, 7.4, 16},
        {ELEMENT_SR, 88, "0(+)", 87.90562, 7.15, 0, 6.42, 0, 6.42, 0.058},
        {ELEMENT_Y, 0, "1/2(-)", 88.90585, 7.75, 1.1, 7.55, 0.15, 7.7, 1.28},
        {ELEMENT_ZR, 0, "", 91.224, 7.16, 0, 6.44, 0.02, 6.46, 0.185},
        {ELEMENT_ZR, 90, "0(+)", 89.9047, 6.4, 0, 5.1, 0, 5.1, 0.011},
        {ELEMENT_ZR, 91, "5/2(+)", 90.90564, 8.7, -1.08, 9.5, 0.15, 9.7, 1.17},
        {ELEMENT_ZR, 92, "0(+)", 91.90504, 7.4, 0, 6.9, 0, 6.9, 0.22},
        {ELEMENT_ZR, 94, "0(+)", 93.90631, 8.2, 0, 8.4, 0, 8.4, 0.0499},
        {ELEMENT_ZR, 96, "0(+)", 95.90828, 5.5, 0, 3.8, 0, 3.8, 0.0229},
        {ELEMENT_NB, 0, "9/2(+)", 92.90638, 7.054, -0.139, 6.253, 0.0024, 6.255, 1.15},
        {ELEMENT_MO, 0, "", 95.94, 6.715, 0, 5.67, 0.04, 5.71, 2.48},
        {ELEMENT_MO, 92, "0(+)", 91.90681, 6.91, 0, 6, 0, 6, 0.019},
        {ELEMENT_MO, 94, "0(+)", 93.90509, 6.8, 0, 5.81, 0, 5.81, 0.015},
        {ELEMENT_MO, 95, "5/2(+)", 94.90584, 6.91, 0, 6, 0.5, 6.5, 13.1},
        {ELEMENT_MO, 96, "0(+)", 95.90468, 6.2, 0, 4.83, 0, 4.83, 0.5},
        {ELEMENT_MO, 97, "5/2(+)", 96.90602, 7.24, 0, 6.59, 0.5, 7.1, 2.5},
        {ELEMENT_MO, 98, "0(+)", 97.90541, 6.58, 0, 5.44, 0, 5.44, 0.127},
        {ELEMENT_MO, 100, "0(+)", 99.90748, 6.73, 0, 5.69, 0, 5.69, 0.4},
        {ELEMENT_TC, 0, "9/2(+)", 98.90625, 6.8, 0, 5.8, 0.5, 6.3, 20},
        {ELEMENT_RU, 0, "", 101.07, 7.21, 0, 6.5, 0.1, 6.6, 2.56},
        {ELEMENT_RU, 96, "0(+)", 95.9076, 0, 0, 0, 0, 0, 0.28},
        {ELEMENT_RU, 98, "0(+)", 97.90529, 0, 0, 0, 0, 0, 0},
        {ELEMENT_RU, 99, "5/2(+)", 98.90594, 0, 0, 0, 0, 0, 6.9},
        {ELEMENT_RU, 100, "0(+)", 99.90422, 0, 0, 0, 0, 0, 4.8},
        {ELEMENT_RU, 101, "5/2(+)", 100.90558, 0, 0, 0, 0, 0, 3.3},
        {ELEMENT_RU, 102, "0(+)", 101.90435, 0, 0, 0, 0, 0, 1.17},
        {ELEMENT_RU, 104, "0(+)", 103.90542, 0, 0, 0, 0, 0, 0.31},
        {ELEMENT_RH, 0, "1/2(-)", 102.9055, 5.88, 0, 4.34, 0.3, 4.6, 144.8},
        {ELEMENT_PD, 0, "", 106.42, 5.91, 0, 4.39, 0.093, 4.48, 6.9},
        {ELEMENT_PD, 102, "0(+)", 101.90563, 7.7, 0, 7.5, 0, 7.5, 3.4},
        {ELEMENT_PD, 104, "0(+)", 103.90403, 7.7, 0, 7.5, 0, 7.5, 0.6},
        {ELEMENT_PD, 105, "5/2(+)", 104.90508, 5.5, -2.6, 3.8, 0.8, 4.6, 20},
        {ELEMENT_PD, 106, "0(+)", 105.90348, 6.4, 0, 5.1, 0, 5.1, 0.304},
        {ELEMENT_PD, 108, "0(+)", 107.9039, 4.1, 0, 2.1, 0, 2.1, 8.5},
        {ELEMENT_PD, 110, "0(+)", 109.90517, 7.7, 0, 7.5, 0, 7.5, 0.226},
        {ELEMENT_AG, 0, "", 107.8682, 5.922, 0, 4.407, 0.58, 4.99, 63.3},
        {ELEMENT_AG, 107, "1/2(-)", 106.90509, 7.555, 1, 7.17, 0.13, 7.3, 37.6},
        {ELEMENT_AG, 109, "1/2(-)", 108.90476, 4.165, -1.6, 2.18, 0.32, 2.5, 91},
        {ELEMENT_CD, 0, "", 112.41, 5.1, 0, 3.3, 2.4, 5.7, 2520},
        {ELEMENT_CD, 106, "0(+)", 105.90646, 0, 0, 0, 0, 0, 1},
        {ELEMENT_CD, 108, "0(+)", 107.90418, 0, 0, 0, 0, 0, 1.1},
        {ELEMENT_CD, 110, "0(+)", 109.90301, 0, 0, 0, 0, 0, 11},
        {ELEMENT_CD, 111, "1/2(+)", 110.90418, 0, 0, 0, 0, 5, 24},
        {ELEMENT_CD, 112, "0(+)", 111.90276, 7.4, 0, 6.9, 0, 6.9, 2.2},
        {ELEMENT_CD, 113, "1/2(+)", 112.9044, -8, 0, 12.1, 0, 0, 20600},
        {ELEMENT_CD, 114, "0(+)", 113.90336, 6.4, 0, 5.1, 0, 5.1, 0.34},
        {ELEMENT_CD, 116, "0(+)", 115.90475, 7.1, 0, 6.3, 0, 6.3, 0.075},
        {ELEMENT_IN, 0, "", 114.82, 4.065, 0, 2.08, 0.54, 2.62, 193.8},
        {ELEMENT_IN, 113, "9/2(+)", 112.90406, 5.39, 0, 3.65, 0, 3.65, 12},
        {ELEMENT_IN, 115, "9/2(+)", 114.90388, 4.01, -2.1, 2.02, 0.55, 2.57, 202},
        {ELEMENT_SN, 0, "", 118.71, 6.225, 0, 4.87, 0.022, 4.892, 0.626},
        {ELEMENT_SN, 112, "0(+)", 111.90483, 6, 0, 4.5, 0, 4.5, 1},
        {ELEMENT_SN, 114, "0(+)", 113.90278, 6.2, 0, 4.8, 0, 4.8, 0.114},
        {ELEMENT_SN, 115, "1/2(+)", 114.90335, 6, 0, 4.5, 0.3, 4.8, 30},
        {ELEMENT_SN, 116, "0(+)", 115.90175, 6, 0, 4.5, 0, 4.5, 0.14},
        {ELEMENT_SN, 117, "1/2(+)", 116.90296, 6.6, 0, 5.5, 0.3, 5.8, 2.3},
        {ELEMENT_SN, 118, "0(+)", 117.90161, 6, 0, 4.5, 0, 4.5, 0.22},
        {ELEMENT_SN, 119, "1/2(+)", 118.90331, 6.2, 0, 4.8, 0.3, 5.1, 2.2},
        {ELEMENT_SN, 120, "0(+)", 119.9022, 6.6, 0, 5.5, 0, 5.5, 0.14},
        {ELEMENT_SN, 122, "0(+)", 121.90344, 5.7, 0, 4.1, 0, 4.1, 0.18},
        {ELEMENT_SN, 124, "0(+)", 123.90527, 6.1, 0, 4.7, 0, 4.7, 0.133},
        {ELEMENT_SB, 0, "", 121.75, 5.57, 0, 3.9, 0, 3.9, 4.91},
        {ELEMENT_SB, 121, "5/2(+)", 120.90382, 5.71, -0.05, 4.1, 0.0003, 4.1, 5.75},
        {ELEMENT_SB, 123, "7/2(+)", 122.90422, 5.38, -0.1, 3.64, 0.001, 3.64, 3.8},
        {ELEMENT_TE, 0, "", 127.6, 5.8, 0, 4.23, 0.09, 4.32, 4.7},
        {ELEMENT_TE, 120, "0(+)", 119.90405, 5.3, 0, 3.5, 0, 3.5, 2.3},
        {ELEMENT_TE, 122, "0(+)", 121.90305, 3.8, 0, 1.8, 0, 1.8, 3.4},
        {ELEMENT_TE, 123, "1/2(+)", 122.90427, -0.05, -2.04, 0.002, 0.52, 0.52, 418},
        {ELEMENT_TE, 124, "0(+)", 123.90282, 7.96, 0, 8, 0, 8, 6.8},
        {ELEMENT_TE, 125, "1/2(+)", 124.90443, 5.02, -0.26, 3.17, 0.008, 3.18, 1.55},
        {ELEMENT_TE, 126, "0(+)", 125.90331, 5.56, 0, 3.88, 0, 3.88, 1.04},
        {ELEMENT_TE, 128, "0(+)", 127.90446, 5.89, 0, 4.36, 0, 4.36, 0.215},
        {ELEMENT_TE, 130, "0(+)", 129.90623, 6.02, 0, 4.55, 0, 4.55, 0.29},
        {ELEMENT_I, 0, "5/2(+)", 126.90447, 5.28, 1.58, 3.5, 0.31, 3.81, 6.15},
        {ELEMENT_XE, 0, "", 131.29, 4.92, 0, 3.04, 0, 0, 23.9},
        {ELEMENT_XE, 124, "0(+)", 123.90589, 0, 0, 0, 0, 0, 165},
        {ELEMENT_XE, 126, "0(+)", 125.90428, 0, 0, 0, 0, 0, 3.5},
        {ELEMENT_XE, 128, "0(+)", 127.90353, 0, 0, 0, 0, 0, 8},
        {ELEMENT_XE, 129, "1/2(+)", 128.90478, 0, 0, 0, 0, 0, 21},
        {ELEMENT_XE, 130, "0(+)", 129.90351, 0, 0, 0, 0, 0, 26},
        {ELEMENT_XE, 131, "3/2(+)", 130.90507, 0, 0, 0, 0, 0, 85},
        {ELEMENT_XE, 132, "0(+)", 131.90414, 0, 0, 0, 0, 0, 0.45},
        {ELEMENT_XE, 134, "0(+)", 133.9054, 0, 0, 0, 0, 0, 0.265},
        {ELEMENT_XE, 136, "0(+)", 135.90721, 0, 0, 0, 0, 0, 0.26},
        {ELEMENT_CS, 0, "7/2(+)", 132.90543, 5.42, 1.29, 3.69, 0.21, 3.9, 29},
        {ELEMENT_BA, 0, "", 137.33, 5.07, 0, 3.23, 0.15, 3.38, 1.1},
        {ELEMENT_BA, 130, "0(+)", 129.90628, -3.6, 0, 1.6, 0, 1.6, 30},
        {ELEMENT_BA, 132, "0(+)", 131.90504, 7.8, 0, 7.6, 0, 7.6, 7},
        {ELEMENT_BA, 134, "0(+)", 133.90449, 5.7, 0, 4.08, 0, 4.08, 2},
        {ELEMENT_BA, 135, "3/2(+)", 134.90567, 4.67, 0, 2.74, 0.5, 3.2, 5.8},
        {ELEMENT_BA, 136, "0(+)", 135.90455, 4.91, 0, 3.03, 0, 3.03, 0.68},
        {ELEMENT_BA, 137, "3/2(+)", 136.90581, 6.83, 0, 5.86, 0.5, 6.4, 3.6},
        {ELEMENT_BA, 138, "0(+)", 137.90523, 4.84, 0, 2.94, 0, 2.94, 0.27},
        {ELEMENT_LA, 0, "", 138.9055, 8.24, 0, 8.53, 1.13, 9.66, 8.97},
        {ELEMENT_LA, 138, "5(+)", 137.90711, 8, 0, 8, 0.5, 8.5, 57},
        {ELEMENT_LA, 139, "7/2(+)", 138.90635, 8.24, 3, 8.53, 1.13, 9.66, 8.93},
        {ELEMENT_CE, 0, "", 140.12, 4.84, 0, 2.94, 0, 2.94, 0.63},
        {ELEMENT_CE, 136, "0(+)", 135.90714, 5.8, 0, 4.23, 0, 4.23, 7.3},
        {ELEMENT_CE, 138, "0(+)", 137.90599, 6.7, 0, 5.64, 0, 5.64, 1.1},
        {ELEMENT_CE, 140, "0(+)", 139.90543, 4.84, 0, 2.94, 0, 2.94, 0.57},
        {ELEMENT_CE, 142, "0(+)", 141.90924, 4.75, 0, 2.84, 0, 2.84, 0.95},
        {ELEMENT_PR, 0, "5/2(+)", 140.90765, 4.45, -0.54, 2.49, 0.037, 2.53, 11.5},
        {ELEMENT_ND, 0, "", 144.24, 7.69, 0, 7.43, 9.2, 16.6, 50.5},
        {ELEMENT_ND, 142, "0(+)", 141.90772, 7.7, 0, 7.5, 0, 7.5, 18.7},
        {ELEMENT_ND, 143, "7/2(-)", 142.90981, 14, 21, 25, 55, 80, 334},
        {ELEMENT_ND, 144, "0(+)", 143.91008, 2.8, 0, 1, 0, 1, 3.6},
        {ELEMENT_ND, 145, "7/2(-)", 144.91257, 14, 0, 25, 5, 30, 42},
        {ELEMENT_ND, 146, "0(+)", 145.91311, 8.7, 0, 9.5, 0, 9.5, 1.4},
        {ELEMENT_ND, 148, "0(+)", 147.91689, 5.7, 0, 4.1, 0, 4.1, 2.5},
        {ELEMENT_ND, 150, "0(+)", 149.92089, 5.3, 0, 3.5, 0, 3.5, 1.2},
        {ELEMENT_PM, 0, "7/2(+)", 146.91514, 12.6, 3.2, 20, 1.3, 21.3, 168.4},
        {ELEMENT_SM, 0, "", 150.36, 0.8, 0, 0.422, 39, 39, 5922},
        {ELEMENT_SM, 144, "0(+)", 143.912, -3, 0, 1, 0, 1, 0.7},
        {ELEMENT_SM, 147, "7/2(-)", 146.9149, 14, 11, 25, 14, 39, 57},
        {ELEMENT_SM, 148, "0(+)", 147.91482, -3, 0, 1, 0, 1, 2.4},
        {ELEMENT_SM, 149, "7/2(-)", 148.91718, -19.2, 31.4, 63.5, 137, 200, 42080},
        {ELEMENT_SM, 150, "0(+)", 149.91727, 14, 0, 25, 0, 25, 104},
        {ELEMENT_SM, 152, "0(+)", 151.91973, -5, 0, 3.1, 0, 3.1, 206},
        {ELEMENT_SM, 154, "0(+)", 153.92221, 9.3, 0, 11, 0, 11, 8.4},
        {ELEMENT_EU, 0, "", 151.96, 7.22, 0, 6.75, 2.5, 9.2, 4530},
        {ELEMENT_EU, 151, "5/2(+)", 150.91985, 6.13, 4.5, 5.5, 3.1, 8.6, 9100},
        {ELEMENT_EU, 153, "5/2(+)", 152.92123, 8.22, 0, 8.5, 1.3, 9.8, 312},
        {ELEMENT_GD, 0, "", 157.25, 6.5, 0, 29.3, 151, 180, 49700},
        {ELEMENT_GD, 152, "0(+)", 151.91979, 10, 0, 13, 0, 13, 735},
        {ELEMENT_GD, 154, "0(+)", 153.92086, 10, 0, 13, 0, 13, 85},
        {ELEMENT_GD, 155, "3/2(-)", 154.92262, 6, 5, 40.8, 25, 66, 61100},
        {ELEMENT_GD, 156, "0(+", 155.92212, 6.3, 0, 5, 0, 5, 1.5},
        {ELEMENT_GD, 157, "3/2(-)", 156.93396, -1.14, 5, 650, 394, 1044, 259000},
        {ELEMENT_GD, 158, "0(+)", 157.9241, 9, 0, 10, 0, 10, 2.2},
        {ELEMENT_GD, 160, "0(+)", 159.92705, 9.15, 0, 10.52, 0, 10.52, 0.77},
        {ELEMENT_TB, 0, "3/2(+)", 158.92534, 7.38, -0.17, 6.84, 0.004, 6.84, 23.4},
        {ELEMENT_DY, 0, "", 162.5, 16.9, 0, 35.9, 54.4, 90.3, 994},
        {ELEMENT_DY, 156, "0(+)", 155.92528, 6.1, 0, 4.7, 0, 4.7, 33},
        {ELEMENT_DY, 158, "0(+)", 157.9244, 6, 0, 5, 0, 5, 43},
        {ELEMENT_DY, 160, "0(+)", 159.92519, 6.7, 0, 5.6, 0, 5.6, 56},
        {ELEMENT_DY, 161, "5/2(+)", 160.92693, 10.3, 0, 13.3, 3, 16, 600},
        {ELEMENT_DY, 162, "0(+)", 161.9268, -1.4, 0, 0.25, 0, 0.25, 194},
        {ELEMENT_DY, 163, "5/2(-)", 162.92873, 5, 1.3, 3.1, 0.21, 3.3, 124},
        {ELEMENT_DY, 164, "0(+)", 163.92917, 49.4, 0, 307, 0, 307, 2840},
        {ELEMENT_HO, 0, "7/2(-)", 164.93032, 8.01, -1.7, 8.06, 0.36, 8.42, 64.7},
        {ELEMENT_ER, 0, "", 167.26, 8.16, 0, 8.37, 0.8, 9.2, 159},
        {ELEMENT_ER, 162, "0(+)", 161.92878, 7, 0, 6, 0, 6, 19},
        {ELEMENT_ER, 164, "0(+)", 163.9292, 8.2, 0, 8.4, 0, 8.4, 13},
        {ELEMENT_ER, 166, "0(+)", 165.93029, 10.3, 0, 13.3, 0, 13.3, 19.6},
        {ELEMENT_ER, 167, "7/2(+)", 166.93205, 3.9, 1, 1.91, 0.13, 2.04, 659},
        {ELEMENT_ER, 168, "0(+)", 167.93237, 8.6, 0, 9.3, 0, 9.3, 2.74},
        {ELEMENT_ER, 170, "0(+)", 169.93546, 9.3, 0, 10.9, 0, 10.9, 5.8},
        {ELEMENT_TM, 0, "1/2(+)", 168.93421, 7.07, 0.9, 6.28, 0.1, 6.38, 100},
        {ELEMENT_YB, 0, "", 173.04, 12.43, 0, 19.42, 4, 23.4, 34.8},
        {ELEMENT_YB, 168, "0(+)", 167.93389, -4.07, 0, 2.13, 0, 2.13, 2230},
        {ELEMENT_YB, 170, "0(+)", 169.93476, 6.77, 0, 5.8, 0, 5.8, 11.4},
        {ELEMENT_YB, 171, "1/2(-)", 170.93632, 9.66, -5.59, 11.7, 3.9, 15.6, 48.6},
        {ELEMENT_YB, 172, "0(+)", 171.93638, 9.43, 0, 11.2, 0, 11.2, 0.8},
        {ELEMENT_YB, 173, "5/2(-)", 172.93821, 9.56, -5.3, 11.5, 3.5, 15, 17.1},
        {ELEMENT_YB, 174, "0(+)", 173.93886, 19.3, 0, 46.8, 0, 46.8, 69.4},
        {ELEMENT_YB, 176, "0(+)", 175.94256, 8.72, 0, 9.6, 0, 9.6, 2.85},
        {ELEMENT_LU, 0, "", 174.967, 7.21, 0, 6.53, 0.7, 7.2, 74},
        {ELEMENT_LU, 175, "7/2(+)", 174.94077, 7.24, 0, 6.59, 0.6, 7.2, 21},
        {ELEMENT_LU, 176, "7(-)", 175.94268, 6.1, 0, 4.7, 1.2, 5.9, 2065},
        {ELEMENT_HF, 0, "", 178.49, 7.77, 0, 7.6, 2.6, 10.2, 104.1},
        {ELEMENT_HF, 174, "0(+)", 173.94004, 10.9, 0, 15, 0, 15, 561},
        {ELEMENT_HF, 176, "0(+)", 175.94141, 6.61, 0, 5.5, 0, 5.5, 23.5},
        {ELEMENT_HF, 177, "7/2(-)", 176.94322, 0.8, 0.9, 0.1, 0.1, 0.2, 373},
        {ELEMENT_HF, 178, "0(+)", 177.9437, 5.9, 0, 4.4, 0, 4.4, 84},
        {ELEMENT_HF, 179, "9/2(+)", 178.94581, 7.46, 0, 7, 0.14, 7.1, 41},
        {ELEMENT_HF, 180, "0(+)", 179.94655, 13.2, 0, 21.9, 0, 21.9, 13.04},
        {ELEMENT_TA, 0, "", 180.9479, 6.91, 0, 6, 0.01, 6.01, 20.6},
        {ELEMENT_TA, 180, "9(-)", 179.94746, 7, 0, 6.2, 0.5, 7, 563},
        {ELEMENT_TA, 181, "7/2(+)", 180.94799, 6.91, -0.29, 6, 0.011, 6.01, 20.5},
        {ELEMENT_W, 0, "", 183.85, 4.86, 0, 2.97, 1.63, 4.6, 18.3},
        {ELEMENT_W, 180, "0(+)", 179.9467, 5, 0, 3, 0, 3, 30},
        {ELEMENT_W, 182, "0(+)", 181.9482, 6.97, 0, 6.1, 0, 6.1, 20.7},
        {ELEMENT_W, 183, "1/2(-)", 182.95022, 6.53, 0, 5.36, 0.3, 5.7, 10.1},
        {ELEMENT_W, 184, "0(+)", 183.95093, 7.48, 0, 7.03, 0, 7.03, 1.7},
        {ELEMENT_W, 186, "0(+)", 185.95436, -0.72, 0, 0.065, 0, 0.065, 37.9},
        {ELEMENT_RE, 0, "", 186.207, 9.2, 0, 10.6, 0.9, 11.5, 89.7},
        {ELEMENT_RE, 185, "5/2(+)", 184.95295, 9, 2, 10.2, 0.5, 10.7, 112},
        {ELEMENT_RE, 187, "5/2(+)", 186.95574, 9.3, 2.8, 10.9, 1, 11.9, 76},
        {ELEMENT_OS, 0, "", 190.2, 10.7, 0, 14.4, 0.3, 14.7, 16},
        {ELEMENT_OS, 184, "0(+)", 183.95249, 10, 0, 13, 0, 13, 3000},
        {ELEMENT_OS, 186, "0(+)", 185.95383, 11.6, 0, 17, 0, 17, 80},
        {ELEMENT_OS, 187, "1/2(-)", 186.95574, 10, 0, 13, 0.3, 13, 320},
        {ELEMENT_OS, 188, "0(+)", 187.95586, 7.6, 0, 7.3, 0, 7.3, 4.7},
        {ELEMENT_OS, 189, "3/2(-)", 188.95814, 10.7, 0, 14.4, 0.5, 14.9, 25},
        {ELEMENT_OS, 190, "0(+)", 189.95844, 11, 0, 15.2, 0, 15.2, 13.1},
        {ELEMENT_OS, 192, "0(+)", 191.96147, 11.5, 0, 16.6, 0, 16.6, 2},
        {ELEMENT_IR, 0, "", 192.22, 10.6, 0, 14.1, 0, 14, 425},
        {ELEMENT_IR, 191, "3/2(+)", 190.96058, 0, 0, 0, 0, 0, 954},
        {ELEMENT_IR, 193, "3/2(+)", 192.96292, 0, 0, 0, 0, 0, 111},
        {ELEMENT_PT, 0, "", 195.08, 9.6, 0, 11.58, 0.13, 11.71, 10.3},
        {ELEMENT_PT, 190, "0(+)", 189.95992, 9, 0, 10, 0, 10, 152},
        {ELEMENT_PT, 192, "0(+)", 191.96102, 9.9, 0, 12.3, 0, 12.3, 10},
        {ELEMENT_PT, 194, "0(+)", 193.96266, 10.55, 0, 14, 0, 14, 1.44},
        {ELEMENT_PT, 195, "1/2(-)", 194.96477, 8.83, -1, 9.8, 0.13, 9.9, 27.5},
        {ELEMENT_PT, 196, "0(+)", 195.96493, 9.89, 0, 12.3, 0, 12.3, 0.72},
        {ELEMENT_PT, 198, "0(+)", 197.96787, 7.8, 0, 7.6, 0, 7.6, 3.66},
        {ELEMENT_AU, 0, "3/2(+)", 196.96654, 7.63, -1.84, 7.32, 0.43, 7.75, 98.65},
        {ELEMENT_HG, 0, "", 200.59, 12.692, 0, 20.24, 6.6, 26.8, 372.3},
        {ELEMENT_HG, 196, "0(+)", 195.96581, 30.3, 0, 115, 0, 115, 3080},
        {ELEMENT_HG, 198, "0(+)", 197.96674, 0, 0, 0, 0, 0, 2},
        {ELEMENT_HG, 199, "1/2(-)", 198.96825, 16.9, 15.5, 36, 30, 66, 2150},
        {ELEMENT_HG, 200, "0(+)", 199.9683, 0, 0, 0, 0, 0, 60},
        {ELEMENT_HG, 201, "3/2(-)", 200.97028, 0, 0, 0, 0, 0, 7.8},
        {ELEMENT_HG, 202, "0(+)", 201.97062, 0, 0, 0, 0, 0, 4.89},
        {ELEMENT_HG, 204, "0(+)", 203.97347, 0, 0, 0, 0, 0, 0.43},
        {ELEMENT_TL, 0, "", 204.383, 8.776, 0, 9.678, 0.21, 9.89, 3.43},
        {ELEMENT_TL, 203, "1/2(+)", 202.97232, 6.99, 1.06, 6.14, 0.14, 6.28, 11.4},
        {ELEMENT_TL, 205, "1/2(+)", 204.9744, 9.52, -0.242, 11.39, 0.007, 11.4, 0.104},
        {ELEMENT_PB, 0, "", 207.2, 9.405, 0, 11.115, 0.003, 11.118, 0.171},
        {ELEMENT_PB, 204, "0(+)", 203.97302, 10.6, 0, 14, 0, 14, 0.65},
        {ELEMENT_PB, 206, "0(+)", 205.97444, 9.23, 0, 10.71, 0, 10.71, 0.03},
        {ELEMENT_PB, 207, "1/2(-)", 206.97587, 9.28, 0.14, 10.82, 0.002, 10.82, 0.699},
        {ELEMENT_PB, 208, "0(+)", 207.97663, 9.5, 0, 11.34, 0, 11.34, 0.00048},
        {ELEMENT_BI, 0, "9/2(-)", 208.98037, 8.532, 0.259, 9.148, 0.0084, 9.156, 0.0338},
        {ELEMENT_RA, 0, "0(+)", 226.0254, 10, 0, 13, 0, 13, 12.8},
        {ELEMENT_TH, 0, "0(+)", 232.03805, 10.52, 0, 13.91, 0, 13.91, 7.37},
        {ELEMENT_PA, 0, "3/2(-)", 231.03588, 9.1, 0, 10.4, 0.1, 10.5, 200.6},
        {ELEMENT_U, 0, "", 238.0289, 8.417, 0, 8.903, 0.005, 8.908, 7.57},
        {ELEMENT_U, 233, "5/2(+)", 233.03963, 10.1, 0, 12.8, 0.1, 12.9, 574},
        {ELEMENT_U, 234, "0(+)", 234.04095, 12.4, 0, 19.3, 0, 19.3, 100.1},
        {ELEMENT_U, 235, "7/2(-)", 235.04392, 10.47, 0, 13.78, 0.2, 14, 680.9},
        {ELEMENT_U, 238, "0(+)", 238.05078, 8.402, 0, 8.871, 0, 8.871, 2.68},
        {ELEMENT_NP, 0, "5/2(+)", 237.04817, 10.55, 0, 14, 0, 14.5, 175.9},
        {ELEMENT_PU, 0, "0(+)", 238.04955, 14.1, 0, 25, 0, 25, 558},
        {ELEMENT_PU, 239, "1/2(+)", 239.05216, 7.7, 0, 7.5, 0.2, 7.7, 1017},
        {ELEMENT_PU, 240, "0(+)", 240.05381, 3.5, 0, 1.54, 0, 1.54, 289.6},
        {ELEMENT_PU, 242, "0(+)", 242.05874, 8.1, 0, 8.2, 0, 8.2, 18.5},
        {ELEMENT_AM, 0, "5/2(-)", 243.06138, 8.3, 0, 8.7, 0.3, 9, 75},
        {ELEMENT_AM, 244, "0(+)", 244.06275, 9.5, 0, 11.3, 0, 11.3, 16.2},
        {ELEMENT_AM, 246, "0(+)", 246.06722, 9.3, 0, 10.9, 0, 10.9, 1.36},
        {ELEMENT_AM, 248, "0(+)", 248.07234, 7.7, 0, 7.5, 0, 7.5, 3},
    };

    if ((Z < 0) || (Z > nElements()))
    {
        Messenger::error("Isotopes::isotopesByElement() - Element with Z={} is out of range!\n", Z);
        return isotopesByElementPrivate_[0];
    }

    return isotopesByElementPrivate_[Z];
}

// Register specified Isotope to given Element
void Isotopes::registerIsotope(Isotope *isotope, int Z) { isotopesByElementPrivate_[Z].own(isotope); }

// Return Isotope with specified A for given Element (if it exists)
Isotope *Isotopes::isotope(int Z, int A)
{
    for (auto *isotope = isotopesByElement(Z).first(); isotope != NULL; isotope = isotope->next())
        if (isotope->A() == A)
            return isotope;

    return NULL;
}

// Return Isotope with specified A for given Element (if it exists)
Isotope *Isotopes::isotope(Element *el, int A)
{
    if (el == NULL)
    {
        Messenger::error("Isotopes::isotope() - Element is NULL.\n");
        return NULL;
    }

    return isotope(el->Z(), A);
}

// Return List of all isotopes available for specified Element
const List<Isotope> &Isotopes::isotopes(int Z) { return isotopesByElement(Z); }

// Return Isotope with specified index (if it exists) in its parent Element
Isotope *Isotopes::isotopeAtIndex(int Z, int index) { return isotopesByElement(Z)[index]; }

// Return natural Isotope for given Element
Isotope *Isotopes::naturalIsotope(Element *el) { return isotope(el, 0); }
