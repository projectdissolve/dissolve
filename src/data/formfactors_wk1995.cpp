/*
	*** Waasmaier & Kirfel '95 X-Ray Form Factors
	*** src/data/formfactors_wk1995.cpp
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

#include "data/formfactors_wk1995.h"
#include "data/formfactors.h"
#include <algorithm>
#include <functional>
#include <vector>

// Constructor
FormFactorData_WK1995::FormFactorData_WK1995(int z, int formalCharge, std::vector<double> a, std::vector<double> b, double c) : FormFactorData(z, formalCharge), a_(a), b_(b), c_(c) {}

// Assignment Operator
FormFactorData_WK1995 &FormFactorData_WK1995::operator=(const FormFactorData_WK1995 &source)
{
	a_ = source.a_;
	b_ = source.b_;
	c_ = source.c_;
	formalCharge_ = source.formalCharge_;

	return (*this);
}

// Return magnitude of form factor at specified Q value
double FormFactorData_WK1995::magnitude(double Q) const
{
	/*
	 * Equation from paper is:
	 *
	 * f = c + SUM[ai*exp(-bi*k^2)]  i=1,5
	 *
	 * k = sin(theta) / lambda
	 *
	 * So, remove factor of 4Pi implicit in our Q value before squaring.
	 */

	const double k = Q / (4 * PI);
	const double k2 = k * k;
	double mag = c_;
	for (int n = 0; n < 5; ++n)
		mag += a_[n] * exp(-b_[0] * k2);

	return mag;
}

namespace XRayFormFactors
{

// Return Waasmaier & Kirfel (1995) form factor data for given element and formal charge (if it exists)
optional<const FormFactorData &> wk1995Data(int Z, int formalCharge)
{
	/*
	 * New Analytical Scattering Factor Functions for Free Atoms and Ions
	 * D. Waasmaier & A. Kirfel
	 * Acta Cryst. (1995). A51, 416-413
	 *
	 * Fit parameters of all atoms/ions (with the exception of O1-)
	 * from publication "New Analytical Scattering Factor Functions for
	 * Free Atoms and Ions", D. Waasmaier & A. Kirfel, Acta Cryst. A 95
	 *
	 * Fit for O1- based on the tabulated values of Table 2 (D.REZ,
	 * P.REZ & I.GRANT, Acta Cryst. (1994), A50, 481-497).
	 *
	 * Fits for all other atoms/ions based on the tabulated values
	 * of Table 6.1.1.1 (atoms) and Table 6.1.1.3 (ions)
	 * (International Tables for Crystallography, Vol. C, 1992).
	 */

	/*
	 * Some elements are tagged with a formal charge equal to "val" (-ence?) type.
	 * These have been removed from the list, but are reproduced here for completeness.
	 *
	 * {ELEMENT_C, Val?, { 1.258489, 0.728215, 1.119856, 2.168133, 0.705239}, {10.683769, 0.208177, 0.836097, 24.603704, 58.954273}, 0.019722},
	 * {ELEMENT_SI, Val?, { 2.879033, 3.072960, 1.515981, 1.390030, 4.995051}, {1.239713, 38.706276, 0.081481, 93.616333, 2.770293}, 0.146030},
	 */

	static const std::vector<FormFactorData_WK1995> wk1995 = {
	    // El  Q  a1  a2  a3  a4  a5  b1  b2  b3  b4  b5  c
	    {ELEMENT_H, 0, {0.413048, 0.294953, 0.187491, 0.080701, 0.023736}, {15.569946, 32.398468, 5.711404, 61.889874, 1.334118}, 0.000049},
	    {ELEMENT_H, 1, {0.206524, 0.147477, 0.093746, 0.040351, 0.011868}, {15.569946, 32.398468, 5.711404, 61.889874, 1.334118}, 0.000025},
	    {ELEMENT_H, -1, {0.702260, 0.763666, 0.248678, 0.261323, 0.023017}, {23.945604, 74.897919, 6.773289, 233.583450, 1.337531}, 0.000425},
	    {ELEMENT_HE, 0, {0.732354, 0.753896, 0.283819, 0.190003, 0.039139}, {11.553918, 4.595831, 1.546299, 26.463964, 0.377523}, 0.000487},
	    {ELEMENT_LI, 0, {0.974637, 0.158472, 0.811855, 0.262416, 0.790108}, {4.334946, 0.342451, 97.102966, 201.363831, 1.409234}, 0.002542},
	    {ELEMENT_LI, 1, {0.432724, 0.549257, 0.376575, -0.336481, 0.976060}, {0.260367, 1.042836, 7.885294, 0.260368, 3.042539}, 0.001764},
	    {ELEMENT_BE, 0, {1.533712, 0.638283, 0.601052, 0.106139, 1.118414}, {42.662079, 0.595420, 99.106499, 0.151340, 1.843093}, 0.002511},
	    {ELEMENT_BE, 2, {3.055430, -2.372617, 1.044914, 0.544233, 0.381737}, {0.001226, 0.001227, 1.542106, 0.456279, 4.047479}, -0.653773},
	    {ELEMENT_B, 0, {2.085185, 1.064580, 1.062788, 0.140515, 0.641784}, {23.494068, 1.137894, 61.238976, 0.114886, 0.399036}, 0.003823},
	    {ELEMENT_C, 0, {2.657506, 1.078079, 1.490909, -4.241070, 0.713791}, {14.780758, 0.776775, 42.086842, -0.000294, 0.239535}, 4.297983},
	    {ELEMENT_N, 0, {11.893780, 3.277479, 1.858092, 0.858927, 0.912985}, {0.000158, 10.232723, 30.344690, 0.656065, 0.217287}, -11.804902},
	    {ELEMENT_O, 0, {2.960427, 2.508818, 0.637853, 0.722838, 1.142756}, {14.182259, 5.936858, 0.112726, 34.958481, 0.390240}, 0.027014},
	    {ELEMENT_O, -1, {3.106934, 3.235142, 1.148886, 0.783981, 0.676953}, {19.868080, 6.960252, 0.170043, 65.693512, 0.630757}, 0.046136},
	    {ELEMENT_O, -2, {3.990247, 2.300563, 0.607200, 1.907882, 1.167080}, {16.639956, 5.636819, 0.108493, 47.299709, 0.379984}, 0.025429},
	    {ELEMENT_F, 0, {3.511943, 2.772244, 0.678385, 0.915159, 1.089261}, {10.687859, 4.380466, 0.093982, 27.255203, 0.313066}, 0.032557},
	    {ELEMENT_F, -1, {0.457649, 3.841561, 1.432771, 0.801876, 3.395041}, {0.917243, 5.507803, 0.164955, 51.076206, 15.821679}, 0.069525},
	    {ELEMENT_NE, 0, {4.183749, 2.905726, 0.520513, 1.135641, 1.228065}, {8.175457, 3.252536, 0.063295, 21.813910, 0.224952}, 0.025576},
	    {ELEMENT_NA, 0, {4.910127, 3.081783, 1.262067, 1.098938, 0.560991}, {3.281434, 9.119178, 0.102763, 132.013947, 0.405878}, 0.079712},
	    {ELEMENT_NA, 1, {3.148690, 4.073989, 0.767888, 0.995612, 0.968249}, {2.594987, 6.046925, 0.070139, 14.122657, 0.217037}, 0.045300},
	    {ELEMENT_MG, 0, {4.708971, 1.194814, 1.558157, 1.170413, 3.239403}, {4.875207, 108.506081, 0.111516, 48.292408, 1.928171}, 0.126842},
	    {ELEMENT_MG, 2, {3.062918, 4.135106, 0.853742, 1.036792, 0.852520}, {2.015803, 4.417941, 0.065307, 9.669710, 0.187818}, 0.058851},
	    {ELEMENT_AL, 0, {4.730796, 2.313951, 1.541980, 1.117564, 3.154754}, {3.628931, 43.051167, 0.095960, 108.932388, 1.555918}, 0.139509},
	    {ELEMENT_AL, 3, {4.132015, 0.912049, 1.102425, 0.614876, 3.219136}, {3.528641, 7.378344, 0.133708, 0.039065, 1.644728}, 0.019397},
	    {ELEMENT_SI, 0, {5.275329, 3.191038, 1.511514, 1.356849, 2.519114}, {2.631338, 33.730728, 0.081119, 86.288643, 1.170087}, 0.145073},
	    {ELEMENT_SI, 4, {3.676722, 3.828496, 1.258033, 0.419024, 0.720421}, {1.446851, 3.013144, 0.064397, 0.206254, 5.970222}, 0.097266},
	    {ELEMENT_P, 0, {1.950541, 4.146930, 1.494560, 1.522042, 5.729711}, {0.908139, 27.044952, 0.071280, 67.520187, 1.981173}, 0.155233},
	    {ELEMENT_S, 0, {6.372157, 5.154568, 1.473732, 1.635073, 1.209372}, {1.514347, 22.092527, 0.061373, 55.445175, 0.646925}, 0.154722},
	    {ELEMENT_CL, 0, {1.446071, 6.870609, 6.151801, 1.750347, 0.634168}, {0.052357, 1.193165, 18.343416, 46.398396, 0.401005}, 0.146773},
	    {ELEMENT_CL, -1, {1.061802, 7.139886, 6.524271, 2.355626, 35.829403}, {0.144727, 1.171795, 19.467655, 60.320301, 0.000436}, -34.916603},
	    {ELEMENT_AR, 0, {7.188004, 6.638454, 0.454180, 1.929593, 1.523654}, {0.956221, 15.339877, 15.339862, 39.043823, 0.062409}, 0.265954},
	    {ELEMENT_K, 0, {8.163991, 7.146945, 1.070140, 0.877316, 1.486434}, {12.816323, 0.808945, 210.327011, 39.597652, 0.052821}, 0.253614},
	    {ELEMENT_K, 1, {-17.609339, 1.494873, 7.150305, 10.899569, 15.808228}, {18.840979, 0.053453, 0.812940, 22.264105, 14.351593}, 0.257164},
	    {ELEMENT_CA, 0, {8.593655, 1.477324, 1.436254, 1.182839, 7.113258}, {10.460644, 0.041891, 81.390381, 169.847839, 0.688098}, 0.196255},
	    {ELEMENT_CA, 2, {8.501441, 12.880483, 9.765095, 7.156669, 0.711160}, {10.525848, -0.004033, 0.010692, 0.684443, 27.231771}, -21.013187},
	    {ELEMENT_SC, 0, {1.476566, 1.487278, 1.600187, 9.177463, 7.099750}, {53.131023, 0.035325, 137.319489, 9.098031, 0.602102}, 0.157765},
	    {ELEMENT_SC, 3, {7.104348, 1.511488, 12.572138, 10.859736, 14.125230}, {0.033386, -53.669773, 38.404816, 24.532240, 0.118642}, 0.601957},
	    {ELEMENT_TI, 0, {9.818524, 1.522646, 1.703101, 1.768774, 7.082555}, {8.001879, 0.029763, 39.885422, 120.157997, 0.532405}, 0.102473},
	    {ELEMENT_TI, 2, {7.040119, 1.496285, 9.657304, 201.800293, 24.039482}, {0.537072, 0.031914, 8.009958, 0.006534, 1.649561}, 0.150362},
	    {ELEMENT_TI, 3, {36.587933, 7.230255, -9.086077, 2.084594, 17.294008}, {0.000681, 0.522262, 5.262317, 15.881716, 6.149805}, -35.111282},
	    {ELEMENT_TI, 4, {45.355537, 7.092900, 7.483858, 10.193876, 0.023064}, {0.523046, 13.082852, -43.498817, 1.678915, -0.110628}, 9.252186},
	    {ELEMENT_V, 0, {10.473575, 1.547881, 1.986381, 1.865616, 7.056250}, {7.081940, 0.026040, 31.909672, 108.022842, 0.474882}, 0.067744},
	    {ELEMENT_V, 2, {7.754356, 2.064100, 2.576998, 2.011404, 7.126177}, {7.066315, 0.014993, 7.066308, 22.055786, 0.467568}, -0.533379},
	    {ELEMENT_V, 3, {9.958480, 1.596350, 1.483442, -10.846044, 17.332867}, {6.763041, 0.056895, 17.750029, 0.328826, 0.388013}, 0.474921},
	    {ELEMENT_V, 5, {15.575018, 8.448095, 1.612040, -9.721855, 1.534029}, {0.682708, 5.566640, 10.527077, 0.907961, 0.066667}, 0.552676},
	    {ELEMENT_CR, 0, {11.007069, 1.555477, 2.985293, 1.347855, 7.034779}, {6.366281, 0.023987, 23.244839, 105.774498, 0.429369}, 0.065510},
	    {ELEMENT_CR, 2, {10.598877, 1.565858, 2.728280, 0.098064, 6.959321}, {6.151846, 0.023519, 17.432816, 54.002388, 0.426301}, 0.049870},
	    {ELEMENT_CR, 3, {7.989310, 1.765079, 2.627125, 1.829380, 6.980908}, {6.068867, 0.018342, 6.068887, 16.309284, 0.420864}, -0.192123},
	    {ELEMENT_MN, 0, {11.709542, 1.733414, 2.673141, 2.023368, 7.003180}, {5.597120, 0.017800, 21.788420, 89.517914, 0.383054}, -0.147293},
	    {ELEMENT_MN, 2, {11.287712, 26.042414, 3.058096, 0.090258, 7.088306}, {5.506225, 0.000774, 16.158575, 54.766354, 0.375580}, -24.566132},
	    {ELEMENT_MN, 3, {6.926972, 2.081342, 11.128379, 2.375107, -0.419287}, {0.378315, 0.015054, 5.379957, 14.429586, 0.004939}, -0.093713},
	    {ELEMENT_MN, 4, {12.409131, 7.466993, 1.809947, -12.138477, 10.780248}, {0.300400, 0.112814, 12.520756, 0.168653, 5.173237}, 0.672146},
	    {ELEMENT_FE, 0, {12.311098, 1.876623, 3.066177, 2.070451, 6.975185}, {5.009415, 0.014461, 18.743040, 82.767876, 0.346506}, -0.304931},
	    {ELEMENT_FE, 2, {11.776765, 11.165097, 3.533495, 0.165345, 7.036932}, {4.912232, 0.001748, 14.166556, 42.381958, 0.341324}, -9.676919},
	    {ELEMENT_FE, 3, {9.721638, 63.403847, 2.141347, 2.629274, 7.033846}, {4.869297, 0.000293, 4.867602, 13.539076, 0.338520}, -61.930725},
	    {ELEMENT_CO, 0, {12.914510, 2.481908, 3.466894, 2.106351, 6.960892}, {4.507138, 0.009126, 16.438129, 76.987320, 0.314418}, -0.936572},
	    {ELEMENT_CO, 2, {6.993840, 26.285812, 12.254289, 0.246114, 4.017407}, {0.310779, 0.000684, 4.400528, 35.741447, 12.536393}, -24.796852},
	    {ELEMENT_CO, 3, {6.861739, 2.678570, 12.281889, 3.501741, -0.179384}, {0.309794, 0.008142, 4.331703, 11.914167, 11.914167}, -1.147345},
	    {ELEMENT_NI, 0, {13.521865, 6.947285, 3.866028, 2.135900, 4.284731}, {4.077277, 0.286763, 14.622634, 71.966080, 0.004437}, -2.762697},
	    {ELEMENT_NI, 2, {12.519017, 37.832058, 4.387257, 0.661552, 6.949072}, {3.933053, 0.000442, 10.449184, 23.860998, 0.283723}, -36.344471},
	    {ELEMENT_NI, 3, {13.579366, 1.902844, 12.859268, 3.811005, -6.838595}, {0.313140, 0.012621, 3.906407, 10.894311, 0.344379}, -0.317618},
	    {ELEMENT_CU, 0, {14.014192, 4.784577, 5.056806, 1.457971, 6.932996}, {3.738280, 0.003744, 13.034982, 72.554794, 0.265666}, -3.254477},
	    {ELEMENT_CU, 1, {12.960763, 16.342150, 1.110102, 5.520682, 6.915452}, {3.576010, 0.000975, 29.523218, 10.114283, 0.261326}, -14.849320},
	    {ELEMENT_CU, 2, {11.895569, 16.344978, 5.799817, 1.048804, 6.789088}, {3.378519, 0.000924, 8.133653, 20.526524, 0.254741}, -14.878383},
	    {ELEMENT_ZN, 0, {14.741002, 6.907748, 4.642337, 2.191766, 38.424042}, {3.388232, 0.243315, 11.903689, 63.312130, 0.000397}, -36.915829},
	    {ELEMENT_ZN, 2, {13.340772, 10.428857, 5.544489, 0.762295, 6.869172}, {3.215913, 0.001413, 8.542680, 21.891756, 0.239215}, -8.945248},
	    {ELEMENT_GA, 0, {15.758946, 6.841123, 4.121016, 2.714681, 2.395246}, {3.121754, 0.226057, 12.482196, 66.203621, 0.007238}, -0.847395},
	    {ELEMENT_GA, 3, {13.123875, 35.288189, 6.126979, 0.611551, 6.724807}, {2.809960, 0.000323, 6.831534, 16.784311, 0.212002}, -33.875122},
	    {ELEMENT_GE, 0, {16.540613, 1.567900, 3.727829, 3.345098, 6.785079}, {2.866618, 0.012198, 13.432163, 58.866047, 0.210974}, 0.018726},
	    {ELEMENT_GE, 4, {6.876636, 6.779091, 9.969591, 3.135857, 0.152389}, {2.025174, 0.176650, 3.573822, 7.685848, 16.677574}, 1.086542},
	    {ELEMENT_AS, 0, {17.025642, 4.503441, 3.715904, 3.937200, 6.790175}, {2.597739, 0.003012, 14.272119, 50.437996, 0.193015}, -2.984117},
	    {ELEMENT_SE, 0, {17.354071, 4.653248, 4.259489, 4.136455, 6.749163}, {2.349787, 0.002550, 15.579460, 45.181202, 0.177432}, -3.160982},
	    {ELEMENT_BR, 0, {17.550570, 5.411882, 3.937180, 3.880645, 6.707793}, {2.119226, 16.557184, 0.002481, 42.164009, 0.162121}, -2.492088},
	    {ELEMENT_BR, -1, {17.714310, 6.466926, 6.947385, 4.402674, -0.697279}, {2.122554, 19.050768, 0.152708, 58.690361, 58.690372}, 1.152674},
	    {ELEMENT_KR, 0, {17.655279, 6.848105, 4.171004, 3.446760, 6.685200}, {1.908231, 16.606236, 0.001598, 39.917473, 0.146896}, -2.810592},
	    {ELEMENT_RB, 0, {8.123134, 2.138042, 6.761702, 1.156051, 17.679546}, {15.142385, 33.542667, 0.129372, 224.132507, 1.713368}, 1.139548},
	    {ELEMENT_RB, 1, {17.684320, 7.761588, 6.680874, 2.668883, 0.070974}, {1.710209, 14.919863, 0.128542, 31.654478, 0.128543}, 1.133263},
	    {ELEMENT_SR, 0, {17.730219, 9.795867, 6.099763, 2.620025, 0.600053}, {1.563060, 14.310868, 0.120574, 135.771317, 0.120574}, 1.140251},
	    {ELEMENT_SR, 2, {17.694973, 1.275762, 6.154252, 9.234786, 0.515995}, {1.550888, 30.133041, 0.118774, 13.821799, 0.118774}, 1.125309},
	    {ELEMENT_Y, 0, {17.792040, 10.253252, 5.714949, 3.170516, 0.918251}, {1.429691, 13.132816, 0.112173, 108.197029, 0.112173}, 1.131787},
	    {ELEMENT_ZR, 0, {17.859772, 10.911038, 5.821115, 3.512513, 0.746965}, {1.310692, 12.319285, 0.104353, 91.777542, 0.104353}, 1.124859},
	    {ELEMENT_ZR, 4, {6.802956, 17.699253, 10.650647, -0.248108, 0.250338}, {0.096228, 1.296127, 11.240715, -0.219259, -0.219021}, 0.827902},
	    {ELEMENT_NB, 0, {17.958399, 12.063054, 5.007015, 3.287667, 1.531019}, {1.211590, 12.246687, 0.098615, 75.011948, 0.098615}, 1.123452},
	    {ELEMENT_NB, 3, {17.714323, 1.675213, 7.483963, 8.322464, 11.143573}, {1.172419, 30.102791, 0.080255, -0.002983, 10.456687}, -8.339573},
	    {ELEMENT_NB, 5, {17.580206, 7.633277, 10.793497, 0.180884, 67.837921}, {1.165852, 0.078558, 9.507652, 31.621656, -0.000438}, -68.024780},
	    {ELEMENT_MO, 0, {6.236218, 17.987711, 12.973127, 3.451426, 0.210899}, {0.090780, 1.108310, 11.468720, 66.684151, 0.090780}, 1.108770},
	    {ELEMENT_MO, 3, {7.447050, 17.778122, 11.886068, 1.997905, 1.789626}, {0.072000, 1.073145, 9.834720, 28.221746, -0.011674}, -1.898764},
	    {ELEMENT_MO, 5, {7.929879, 17.667669, 11.515987, 0.500402, 77.444084}, {0.068856, 1.068064, 9.046229, 26.558945, -0.000473}, -78.056595},
	    {ELEMENT_MO, 6, {34.757683, 9.653037, 6.584769, -18.628115, 2.490594}, {1.301770, 7.123843, 0.094097, 1.617443, 12.335434}, 1.141916},
	    {ELEMENT_TC, 0, {17.840963, 3.428236, 1.373012, 12.947364, 6.335469}, {1.005729, 41.901382, 119.320541, 9.781542, 0.083391}, 1.074784},
	    {ELEMENT_RU, 0, {6.271624, 17.906738, 14.123269, 3.746008, 0.908235}, {0.077040, 0.928222, 9.555345, 35.860680, 123.552246}, 1.043992},
	    {ELEMENT_RU, 3, {17.894758, 13.579529, 10.729251, 2.474095, 48.227997}, {0.902827, 8.740579, 0.045125, 24.764954, -0.001699}, -51.905243},
	    {ELEMENT_RU, 4, {17.845776, 13.455084, 10.229087, 1.653524, 14.059795}, {0.901070, 8.482392, 0.045972, 23.015272, -0.004889}, -17.241762},
	    {ELEMENT_RH, 0, {6.216648, 17.919739, 3.854252, 0.840326, 15.173498}, {0.070789, 0.856121, 33.889484, 121.686691, 9.029517}, 0.995452},
	    {ELEMENT_RH, 3, {17.758621, 14.569813, 5.298320, 2.533579, 0.879753}, {0.841779, 8.319533, 0.069050, 23.709131, 0.069050}, 0.960843},
	    {ELEMENT_RH, 4, {17.716188, 14.446654, 5.185801, 1.703448, 0.989992}, {0.840572, 8.100647, 0.068995, 22.357307, 0.068995}, 0.959941},
	    {ELEMENT_PD, 0, {6.121511, 4.784063, 16.631683, 4.318258, 13.246773}, {0.062549, 0.784031, 8.751391, 34.489983, 0.784031}, 0.883099},
	    {ELEMENT_PD, 2, {6.122282, 15.651012, 3.513508, 9.060790, 8.771199}, {0.062424, 8.018296, 24.784275, 0.776457, 0.776457}, 0.879336},
	    {ELEMENT_PD, 4, {6.152421, -96.069023, 31.622141, 81.578255, 17.801403}, {0.063951, 11.090354, 13.466152, 9.758302, 0.783014}, 0.915874},
	    {ELEMENT_AG, 0, {6.073874, 17.155437, 4.173344, 0.852238, 17.988686}, {0.055333, 7.896512, 28.443739, 110.376106, 0.716809}, 0.756603},
	    {ELEMENT_AG, 1, {6.091192, 4.019526, 16.948174, 4.258638, 13.889437}, {0.056305, 0.719340, 7.758938, 27.368349, 0.719340}, 0.785127},
	    {ELEMENT_AG, 2, {6.401808, 48.699802, 4.799859, -32.332523, 16.356710}, {0.068167, 0.942270, 20.639496, 1.100365, 6.883131}, 1.068247},
	    {ELEMENT_CD, 0, {6.080986, 18.019468, 4.018197, 1.303510, 17.974669}, {0.048990, 7.273646, 29.119284, 95.831207, 0.661231}, 0.603504},
	    {ELEMENT_CD, 2, {6.093711, 43.909691, 17.041306, -39.675117, 17.958918}, {0.050624, 8.654143, 15.621396, 11.082067, 0.667591}, 0.664795},
	    {ELEMENT_IN, 0, {6.196477, 18.816183, 4.050479, 1.638929, 17.962912}, {0.042072, 6.695665, 31.009790, 103.284348, 0.610714}, 0.333097},
	    {ELEMENT_IN, 3, {6.206277, 18.497746, 3.078131, 10.524613, 7.401234}, {0.041357, 6.605563, 18.792250, 0.608082, 0.608082}, 0.293677},
	    {ELEMENT_SN, 0, {19.325171, 6.281571, 4.498866, 1.856934, 17.917318}, {6.118104, 0.036915, 32.529045, 95.037186, 0.565651}, 0.119024},
	    {ELEMENT_SN, 2, {6.353672, 4.770377, 14.672025, 4.235959, 18.002131}, {0.034720, 6.167891, 6.167879, 29.006456, 0.561774}, -0.042519},
	    {ELEMENT_SN, 4, {15.445732, 6.420892, 4.562980, 1.713385, 18.033537}, {6.280898, 0.033144, 6.280899, 17.983601, 0.557980}, -0.172219},
	    {ELEMENT_SB, 0, {5.394956, 6.549570, 19.650681, 1.827820, 17.867832}, {33.326523, 0.030974, 5.564929, 87.130966, 0.523992}, -0.290506},
	    {ELEMENT_SB, 3, {10.189171, 57.461918, 19.356573, 4.862206, -45.394096}, {0.089485, 0.375256, 5.357987, 22.153736, 0.297768}, 1.516108},
	    {ELEMENT_SB, 5, {17.920622, 6.647932, 12.724075, 1.555545, 7.600591}, {0.522315, 0.029487, 5.718210, 16.433775, 5.718204}, -0.445371},
	    {ELEMENT_TE, 0, {6.660302, 6.940756, 19.847015, 1.557175, 17.802427}, {33.031654, 0.025750, 5.065547, 84.101616, 0.487660}, -0.806668},
	    {ELEMENT_I, 0, {19.884502, 6.736593, 8.110516, 1.170953, 17.548716}, {4.628591, 0.027754, 31.849096, 84.406387, 0.463550}, -0.448811},
	    {ELEMENT_I, -1, {20.010330, 17.835524, 8.104130, 2.231118, 9.158548}, {4.565931, 0.444266, 32.430672, 95.149040, 0.014906}, -3.341004},
	    {ELEMENT_XE, 0, {19.978920, 11.774945, 9.332182, 1.244749, 17.737501}, {4.143356, 0.010142, 28.796200, 75.280685, 0.413616}, -6.065902},
	    {ELEMENT_CS, 0, {17.418674, 8.314444, 10.323193, 1.383834, 19.876251}, {0.399828, 0.016872, 25.605827, 233.339676, 3.826915}, -2.322802},
	    {ELEMENT_CS, 1, {19.939056, 24.967621, 10.375884, 0.454243, 17.660248}, {3.770511, 0.004040, 25.311275, 76.537766, 0.384730}, -19.394306},
	    {ELEMENT_BA, 0, {19.747343, 17.368477, 10.465718, 2.592602, 11.003653}, {3.481823, 0.371224, 21.226641, 173.834274, 0.010719}, -5.183497},
	    {ELEMENT_BA, 2, {19.750200, 17.513683, 10.884892, 0.321585, 65.149834}, {3.430748, 0.361590, 21.358307, 70.309402, 0.001418}, -59.618172},
	    {ELEMENT_LA, 0, {19.966019, 27.329655, 11.018425, 3.086696, 17.335455}, {3.197408, 0.003446, 19.955492, 141.381973, 0.341817}, -21.745489},
	    {ELEMENT_LA, 3, {19.688887, 17.345703, 11.356296, 0.099418, 82.358124}, {3.146211, 0.339586, 18.753832, 90.345459, 0.001072}, -76.846909},
	    {ELEMENT_CE, 0, {17.355122, 43.988499, 20.546650, 3.130670, 11.353665}, {0.328369, 0.002047, 3.088196, 134.907654, 18.832960}, -38.386017},
	    {ELEMENT_CE, 3, {26.593231, 85.866432, -6.677695, 12.111847, 17.401903}, {3.280381, 0.001012, 4.313575, 17.868504, 0.326962}, -80.313423},
	    {ELEMENT_CE, 4, {17.457533, 25.659941, 11.691037, 19.695251, -16.994749}, {0.311812, -0.003793, 16.568687, 2.886395, -0.008931}, -3.515096},
	    {ELEMENT_PR, 0, {21.551311, 17.161730, 11.903859, 2.679103, 9.564197}, {2.995675, 0.312491, 17.716705, 152.192825, 0.010468}, -3.871068},
	    {ELEMENT_PR, 3, {20.879841, 36.035797, 12.135341, 0.283103, 17.167803}, {2.870897, 0.002364, 16.615236, 53.909359, 0.306993}, -30.500784},
	    {ELEMENT_PR, 4, {17.496082, 21.538509, 20.403114, 12.062211, -7.492043}, {0.294457, -0.002742, 2.772886, 15.804613, -0.013556}, -9.016722},
	    {ELEMENT_ND, 0, {17.331244, 62.783924, 12.160097, 2.663483, 22.239950}, {0.300269, 0.001320, 17.026001, 148.748993, 2.910268}, -57.189842},
	    {ELEMENT_ND, 3, {17.120077, 56.038139, 21.468307, 10.000671, 2.905866}, {0.291295, 0.001421, 2.743681, 14.581367, 22.485098}, -50.541992},
	    {ELEMENT_PM, 0, {17.286388, 51.560162, 12.478557, 2.675515, 22.960947}, {0.286620, 0.001550, 16.223755, 143.984512, 2.796480}, -45.973682},
	    {ELEMENT_PM, 3, {22.221066, 17.068142, 12.805423, 0.435687, 52.238770}, {2.635767, 0.277039, 14.927315, 45.768017, 0.001455}, -46.767181},
	    {ELEMENT_SM, 0, {23.700363, 23.072214, 12.777782, 2.684217, 17.204367}, {2.689539, 0.003491, 15.495437, 139.862473, 0.274536}, -17.452166},
	    {ELEMENT_SM, 3, {15.618565, 19.538092, 13.398946, -4.358811, 24.490461}, {0.006001, 0.306379, 14.979594, 0.748825, 2.454492}, -9.714854},
	    {ELEMENT_EU, 0, {17.186195, 37.156837, 13.103387, 2.707246, 24.419271}, {0.261678, 0.001995, 14.787360, 134.816299, 2.581883}, -31.586687},
	    {ELEMENT_EU, 2, {23.899035, 31.657497, 12.955752, 1.700576, 16.992199}, {2.467332, 0.002230, 13.625002, 35.089481, 0.253136}, -26.204315},
	    {ELEMENT_EU, 3, {17.758327, 33.498665, 24.067188, 13.436883, -9.019134}, {0.244474, -0.003901, 2.487526, 14.568011, -0.015628}, -19.768026},
	    {ELEMENT_GD, 0, {24.898117, 17.104952, 13.222581, 3.266152, 48.995213}, {2.435028, 0.246961, 13.996325, 110.863091, 0.001383}, -43.505684},
	    {ELEMENT_GD, 3, {24.344999, 16.945311, 13.866931, 0.481674, 93.506378}, {2.333971, 0.239215, 12.982995, 43.876347, 0.000673}, -88.147179},
	    {ELEMENT_TB, 0, {25.910013, 32.344139, 13.765117, 2.751404, 17.064405}, {2.373912, 0.002034, 13.481969, 125.836510, 0.236916}, -26.851971},
	    {ELEMENT_TB, 3, {24.878252, 16.856016, 13.663937, 1.279671, 39.271294}, {2.223301, 0.227290, 11.812528, 29.910065, 0.001527}, -33.950317},
	    {ELEMENT_DY, 0, {26.671785, 88.687576, 14.065445, 2.768497, 17.067781}, {2.282593, 0.000665, 12.920230, 121.937187, 0.225531}, -83.279831},
	    {ELEMENT_DY, 3, {16.864344, 90.383461, 13.675473, 1.687078, 25.540651}, {0.216275, 0.000593, 11.121207, 26.250975, 2.135930}, -85.150650},
	    {ELEMENT_HO, 0, {27.150190, 16.999819, 14.059334, 3.386979, 46.546471}, {2.169660, 0.215414, 12.213148, 100.506783, 0.001211}, -41.165253},
	    {ELEMENT_HO, 3, {16.837524, 63.221336, 13.703766, 2.061602, 26.202621}, {0.206873, 0.000796, 10.500283, 24.031883, 2.055060}, -58.026505},
	    {ELEMENT_ER, 0, {28.174887, 82.493271, 14.624002, 2.802756, 17.018515}, {2.120995, 0.000640, 11.915256, 114.529938, 0.207519}, -77.135223},
	    {ELEMENT_ER, 3, {16.810127, 22.681061, 13.864114, 2.294506, 26.864477}, {0.198293, 0.002126, 9.973341, 22.836388, 1.979442}, -17.513460},
	    {ELEMENT_TM, 0, {28.925894, 76.173798, 14.904704, 2.814812, 16.998117}, {2.046203, 0.000656, 11.465375, 111.411980, 0.199376}, -70.839813},
	    {ELEMENT_TM, 3, {16.787500, 15.350905, 14.182357, 2.299111, 27.573771}, {0.190852, 0.003036, 9.602934, 22.526880, 1.912862}, -10.192087},
	    {ELEMENT_YB, 0, {29.676760, 65.624069, 15.160854, 2.830288, 16.997850}, {1.977630, 0.000720, 11.044622, 108.139153, 0.192110}, -60.313812},
	    {ELEMENT_YB, 2, {28.443794, 16.849527, 14.165081, 3.445311, 28.308853}, {1.863896, 0.183811, 9.225469, 23.691355, 0.001463}, -23.214935},
	    {ELEMENT_YB, 3, {28.191629, 16.828087, 14.167848, 2.744962, 23.171774}, {1.842889, 0.182788, 9.045957, 20.799847, 0.001759}, -18.103676},
	    {ELEMENT_LU, 0, {30.122866, 15.099346, 56.314899, 3.540980, 16.943729}, {1.883090, 10.342764, 0.000780, 89.559250, 0.183849}, -51.049416},
	    {ELEMENT_LU, 3, {28.828693, 16.823227, 14.247617, 3.079559, 25.647667}, {1.776641, 0.175560, 8.575531, 19.693701, 0.001453}, -20.626528},
	    {ELEMENT_HF, 0, {30.617033, 15.145351, 54.933548, 4.096253, 16.896156}, {1.795613, 9.934469, 0.000739, 76.189705, 0.175914}, -49.719837},
	    {ELEMENT_HF, 4, {29.267378, 16.792543, 14.785310, 2.184128, 23.791996}, {1.697911, 0.168313, 8.190025, 18.277578, 0.001431}, -18.820383},
	    {ELEMENT_TA, 0, {31.066359, 15.341823, 49.278297, 4.577665, 16.828321}, {1.708732, 9.618455, 0.000760, 66.346199, 0.168002}, -44.119026},
	    {ELEMENT_TA, 5, {29.539469, 16.741854, 15.182070, 1.642916, 16.437447}, {1.612934, 0.160460, 7.654408, 17.070732, 0.001858}, -11.542459},
	    {ELEMENT_W, 0, {31.507900, 15.682498, 37.960129, 4.885509, 16.792112}, {1.629485, 9.446448, 0.000898, 59.980675, 0.160798}, -32.864574},
	    {ELEMENT_W, 6, {29.729357, 17.247808, 15.184488, 1.154652, 0.739335}, {1.501648, 0.140803, 6.880573, 14.299601, 14.299618}, 3.945157},
	    {ELEMENT_RE, 0, {31.888456, 16.117104, 42.390297, 5.211669, 16.767591}, {1.549238, 9.233474, 0.000689, 54.516373, 0.152815}, -37.412682},
	    {ELEMENT_OS, 0, {32.210297, 16.678440, 48.559906, 5.455839, 16.735533}, {1.473531, 9.049695, 0.000519, 50.210201, 0.145771}, -43.677956},
	    {ELEMENT_OS, 4, {17.113485, 15.792370, 23.342392, 4.090271, 7.671292}, {0.131850, 7.288542, 1.389307, 19.629425, 1.389307}, 3.988390},
	    {ELEMENT_IR, 0, {32.004436, 1.975454, 17.070105, 15.939454, 5.990003}, {1.353767, 81.014175, 0.128093, 7.661196, 26.659403}, 4.018893},
	    {ELEMENT_IR, 3, {31.537575, 16.363338, 15.597141, 5.051404, 1.436935}, {1.334144, 7.451918, 0.127514, 21.705648, 0.127515}, 4.009459},
	    {ELEMENT_IR, 4, {30.391249, 16.146996, 17.019068, 4.458904, 0.975372}, {1.328519, 7.181766, 0.127337, 19.060146, 1.328519}, 4.006865},
	    {ELEMENT_PT, 0, {31.273891, 18.445440, 17.063745, 5.555933, 1.575270}, {1.316992, 8.797154, 0.124741, 40.177994, 1.316997}, 4.050394},
	    {ELEMENT_PT, 2, {31.986849, 17.249048, 15.269374, 5.760234, 1.694079}, {1.281143, 7.625512, 0.123571, 24.190826, 0.123571}, 4.032512},
	    {ELEMENT_PT, 4, {41.932713, 16.339224, 17.653894, 6.012420, 0.778721}, {6.466086, 0.128917, 16.954155, -12.036877, 4.094551}, 1.111409},
	    {ELEMENT_AU, 0, {16.777390, 19.317156, 32.979683, 5.595453, 10.576854}, {0.122737, 8.621570, 1.256902, 38.008820, 0.000601}, -6.279078},
	    {ELEMENT_AU, 1, {32.124306, 16.716476, 16.814100, 7.311565, 0.993064}, {1.216073, 7.165378, 0.118715, 20.442486, 53.095985}, 4.040792},
	    {ELEMENT_AU, 3, {31.704271, 17.545767, 16.819551, 5.522640, 0.361725}, {1.215561, 7.220506, 0.118812, 20.050970, 1.215562}, 4.042679},
	    {ELEMENT_HG, 0, {16.839890, 20.023823, 28.428564, 5.881564, 4.714706}, {0.115905, 8.256927, 1.195250, 39.247227, 1.195250}, 4.076478},
	    {ELEMENT_HG, 1, {28.866837, 19.277540, 16.776051, 6.281459, 3.710289}, {1.173967, 7.583842, 0.115351, 29.055994, 1.173968}, 4.068430},
	    {ELEMENT_HG, 2, {32.411079, 18.690371, 16.711773, 9.974835, -3.847611}, {1.162980, 7.329806, 0.114518, 22.009489, 22.009493}, 4.052869},
	    {ELEMENT_TL, 0, {16.630795, 19.386616, 32.808571, 1.747191, 6.356862}, {0.110704, 7.181401, 1.119730, 90.660263, 26.014978}, 4.066939},
	    {ELEMENT_TL, 1, {32.295044, 16.570049, 17.991013, 1.535355, 7.554591}, {1.101544, 0.110020, 6.528559, 52.495068, 20.338634}, 4.054030},
	    {ELEMENT_TL, 3, {32.525639, 19.139185, 17.100321, 5.891115, 12.599463}, {1.094966, 6.900992, 0.103667, 18.489614, -0.001401}, -9.256075},
	    {ELEMENT_PB, 0, {16.419567, 32.738590, 6.530247, 2.342742, 19.916475}, {0.105499, 1.055049, 25.025890, 80.906593, 6.664449}, 4.049824},
	    {ELEMENT_PB, 2, {27.392647, 16.496822, 19.984501, 6.813923, 5.233910}, {1.058874, 0.106305, 6.708123, 24.395554, 1.058874}, 4.065623},
	    {ELEMENT_PB, 4, {32.505657, 20.014240, 14.645661, 5.029499, 1.760138}, {1.047035, 6.670321, 0.105279, 16.525040, 0.105279}, 4.044678},
	    {ELEMENT_BI, 0, {16.282274, 32.725136, 6.678302, 2.694750, 20.576559}, {0.101180, 1.002287, 25.714146, 77.057549, 6.291882}, 4.040914},
	    {ELEMENT_BI, 3, {32.461437, 19.438683, 16.302486, 7.322662, 0.431704}, {0.997930, 6.038867, 0.101338, 18.371586, 46.361046}, 4.043703},
	    {ELEMENT_BI, 5, {16.734028, 20.580494, 9.452623, 61.155834, -34.041023}, {0.105076, 4.773282, 11.762162, 1.211775, 1.619408}, 4.113663},
	    {ELEMENT_PO, 0, {16.289164, 32.807171, 21.095163, 2.505901, 7.254589}, {0.098121, 0.966265, 6.046622, 76.598068, 28.096128}, 4.046556},
	    {ELEMENT_AT, 0, {16.011461, 32.615547, 8.113899, 2.884082, 21.377867}, {0.092639, 0.904416, 26.543257, 68.372963, 5.499512}, 3.995684},
	    {ELEMENT_RN, 0, {16.070229, 32.641106, 21.489658, 2.299218, 9.480184}, {0.090437, 0.876409, 5.239687, 69.188477, 27.632641}, 4.020977},
	    {ELEMENT_FR, 0, {16.007385, 32.663830, 21.594351, 1.598497, 11.121192}, {0.087031, 0.840187, 4.954467, 199.805801, 26.905106}, 4.003472},
	    {ELEMENT_RA, 0, {32.563690, 21.396671, 11.298093, 2.834688, 15.914965}, {0.801980, 4.590666, 22.758972, 160.404388, 0.083544}, 3.981773},
	    {ELEMENT_RA, 2, {4.986228, 32.474945, 21.947443, 11.800013, 10.807292}, {0.082597, 0.791468, 4.608034, 24.792431, 0.082597}, 3.956572},
	    {ELEMENT_AC, 0, {15.914053, 32.535042, 21.553976, 11.433394, 3.612409}, {0.080511, 0.770669, 4.352206, 21.381622, 130.500748}, 3.939212},
	    {ELEMENT_AC, 3, {15.584983, 32.022125, 21.456327, 0.757593, 12.341252}, {0.077438, 0.739963, 4.040735, 47.525002, 19.406845}, 3.838984},
	    {ELEMENT_TH, 0, {15.784024, 32.454899, 21.849222, 4.239077, 11.736191}, {0.077067, 0.735137, 4.097976, 109.464111, 20.512138}, 3.922533},
	    {ELEMENT_TH, 4, {15.515445, 32.090691, 13.996399, 12.918157, 7.635514}, {0.074499, 0.711663, 3.871044, 18.596891, 3.871044}, 3.831122},
	    {ELEMENT_PA, 0, {32.740208, 21.973675, 12.957398, 3.683832, 15.744058}, {0.709545, 4.050881, 19.231543, 117.255005, 0.074040}, 3.886066},
	    {ELEMENT_U, 0, {15.679275, 32.824306, 13.660459, 3.687261, 22.279434}, {0.071206, 0.681177, 18.236156, 112.500038, 3.930325}, 3.854444},
	    {ELEMENT_U, 3, {15.360309, 32.395657, 21.961290, 1.325894, 14.251453}, {0.067815, 0.654643, 3.643409, 39.604965, 16.330570}, 3.706622},
	    {ELEMENT_U, 4, {15.355091, 32.235306, 0.557745, 14.396367, 21.751173}, {0.067789, 0.652613, 42.354237, 15.908239, 3.553231}, 3.705863},
	    {ELEMENT_U, 6, {15.333844, 31.770849, 21.274414, 13.872636, 0.048519}, {0.067644, 0.646384, 3.317894, 14.650250, 75.339699}, 3.700591},
	    {ELEMENT_NP, 0, {32.999901, 22.638077, 14.219973, 3.672950, 15.683245}, {0.657086, 3.854918, 17.435474, 109.464485, 0.068033}, 3.769391},
	    {ELEMENT_NP, 3, {15.378152, 32.572132, 22.206125, 1.413295, 14.828381}, {0.064613, 0.631420, 3.561936, 37.875511, 15.546129}, 3.603370},
	    {ELEMENT_NP, 4, {15.373926, 32.423019, 21.969994, 0.662078, 14.969350}, {0.064597, 0.629658, 3.476389, 39.438942, 15.135764}, 3.603039},
	    {ELEMENT_NP, 6, {15.359986, 31.992825, 21.412458, 0.066574, 14.568174}, {0.064528, 0.624505, 3.253441, 67.658318, 13.980832}, 3.600942},
	    {ELEMENT_PU, 0, {33.281178, 23.148544, 15.153755, 3.031492, 15.704215}, {0.634999, 3.856168, 16.849735, 121.292038, 0.064857}, 3.664200},
	    {ELEMENT_PU, 3, {15.356004, 32.769127, 22.680210, 1.351055, 15.416232}, {0.060590, 0.604663, 3.491509, 37.260635, 14.981921}, 3.428895},
	    {ELEMENT_PU, 4, {15.416219, 32.610569, 22.256662, 0.719495, 15.518152}, {0.061456, 0.607938, 3.411848, 37.628792, 14.464360}, 3.480408},
	    {ELEMENT_PU, 6, {15.436506, 32.289719, 14.726737, 15.012391, 7.024677}, {0.061815, 0.606541, 3.245363, 13.616438, 3.245364}, 3.502325},
	    {ELEMENT_AM, 0, {33.435162, 23.657259, 15.576339, 3.027023, 15.746100}, {0.612785, 3.792942, 16.195778, 117.757004, 0.061755}, 3.541160},
	    {ELEMENT_CM, 0, {15.804837, 33.480801, 24.150198, 3.655563, 15.499866}, {0.058619, 0.590160, 3.674720, 100.736191, 15.408296}, 3.390840},
	    {ELEMENT_BK, 0, {15.889072, 33.625286, 24.710381, 3.707139, 15.839268}, {0.055503, 0.569571, 3.615472, 97.694786, 14.754303}, 3.213169},
	    {ELEMENT_CF, 0, {33.794075, 25.467693, 16.048487, 3.657525, 16.008982}, {0.550447, 3.581973, 14.357388, 96.064972, 0.052450}, 3.005326}};

	auto it = std::find_if(wk1995.cbegin(), wk1995.cend(), [&](const FormFactorData_WK1995 &data) { return data.Z() == Z && data.formalCharge() == formalCharge; });
	return std::make_tuple(std::ref(*it), it == wk1995.end());
}

} // namespace XRayFormFactors
