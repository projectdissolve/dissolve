/*
	*** SQ Module - Options
	*** src/modules/sq/options.cpp
	Copyright T. Youngs 2012-2019

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

#include "modules/sq/sq.h"
#include "keywords/types.h"
#include "main/dissolve.h"
#include "classes/species.h"
#include "base/lineparser.h"
#include "templates/enumhelpers.h"
#include "genericitems/listhelper.h"

// Set up keywords for Module
void SQModule::setUpKeywords()
{
	frequency_ = 5;

	// Q range
	keywords_.add("Calculation", new DoubleKeyword(0.05, 1.0e-5), "QDelta", "Step size in Q for S(Q) calculation");
	keywords_.add("Calculation", new DoubleKeyword(-1.0, -1.0), "QMax", "Maximum Q for calculated S(Q)");
	keywords_.add("Calculation", new DoubleKeyword(0.01, 0.0), "QMin", "Minimum Q for calculated S(Q)");
	keywords_.add("Calculation", new BroadeningFunctionKeyword(BroadeningFunction()), "QBroadening", "Instrument broadening function to apply when calculating S(Q)");
	keywords_.add("Calculation", new WindowFunctionKeyword(WindowFunction(WindowFunction::Lorch0Window)), "WindowFunction", "Window function to apply in Fourier-transform of g(r) to S(Q)");

	// Export
	keywords_.add("Export", new BoolKeyword(false), "Save", "Whether to save partials to disk after calculation", "<True|False>");
}

// Parse keyword line, returning true (1) on success, false (0) for recognised but failed, and -1 for not recognised
int SQModule::parseComplexKeyword(KeywordBase* keyword, LineParser& parser, Dissolve* dissolve, GenericList& targetList, const char* prefix)
{
	return -1;
}
