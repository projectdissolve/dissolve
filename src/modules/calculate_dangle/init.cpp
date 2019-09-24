/*
	*** Calculate Distance-Angle Module - Initialisation
	*** src/modules/calculate_dangle/init.cpp
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

#include "modules/calculate_dangle/dangle.h"
#include "keywords/types.h"
#include "procedure/nodes/calculateangle.h"
#include "procedure/nodes/calculatedistance.h"
#include "procedure/nodes/collect1d.h"
#include "procedure/nodes/collect2d.h"
#include "procedure/nodes/operatenormalise.h"
#include "procedure/nodes/operatenumberdensitynormalise.h"
#include "procedure/nodes/operatesitepopulationnormalise.h"
#include "procedure/nodes/operatesphericalshellnormalise.h"
#include "procedure/nodes/process1d.h"
#include "procedure/nodes/process2d.h"
#include "procedure/nodes/select.h"

// Perform any necessary initialisation for the Module
void CalculateDAngleModule::initialise()
{
	/*
	 * Assemble the following Procedure:
	 *
	 * Select  'A'
	 *   Site  ...
	 *   ForEach
	 *     Select  'B'
	 *       Site  ...
	 *       SameMoleculeAsSite  'A'
	 *       ForEach
	 *         Select  'C'
	 *           Site  ...
	 *           ExcludeSameMolecule  'A'
	 *           ForEach
	 *             Calculate  'rBC'
	 *               Distance  'B'  'C'
	 *             EndCalculate
	 *             Calculate  'aABC'
	 *               Angle  'A'  'B'  'C'
	 *             EndCalculate
	 *             Collect2D  DAngle
	 *               QuantityX  'rBC'
	 *               QuantityY  'aABC'
	 *               RangeX  0.0  10.0  0.05
	 *               RangeY  0.0  180.0  1.0
	 *               SubCollect
	 *                 Collect1D  'RDF(BC)'
	 *                   QuantityX  'rBC'
	 *                   RangeX  0.0  10.0  0.05
	 *                 EndCollect
	 *                 Collect1D  'ANGLE(ABC)'
	 *                   QuantityX  'aABC'
	 *                   RangeX  0.0  180.0  1.0
	 *                 EndCollect
	 *               EndSubCollect
	 *             EndCollect2D
	 *           EndForEach  'C'
	 *         EndSelect  'C'
	 *       EndForEach  'B'
	 *     EndSelect  'B'
	 *   EndForEach  'A'
	 * EndSelect  'A'
	 * Process2D  DAngle
	 *   NormaliseToOne  On
	 *   LabelValue  'g(r)'
	 *   LabelX  'r, Angstroms'
	 *   LabelY  'theta, Degrees'
	 *   LabelValue  'Probability'
	 * EndProcess2D
	 */

	// Select: Site 'A'
	selectA_ = new SelectProcedureNode;
	selectA_->setName("A");
	SequenceProcedureNode* forEachA = selectA_->addForEachBranch(ProcedureNode::AnalysisContext);
	analyser_.addRootSequenceNode(selectA_);

	// -- Select: Site 'B'
	selectB_ = new SelectProcedureNode;
	selectB_->setName("B");
	selectB_->setKeyword<SelectProcedureNode*>("SameMoleculeAsSite", selectA_);
	SequenceProcedureNode* forEachB = selectB_->addForEachBranch(ProcedureNode::AnalysisContext);
	forEachA->addNode(selectB_);

	// -- -- Select: Site 'C'
	selectC_ = new SelectProcedureNode;
	selectC_->setName("C");
	RefList<SelectProcedureNode> sameMoleculeExclusions(selectA_);
	selectC_->setKeyword< RefList<SelectProcedureNode>& >("ExcludeSameMolecule", sameMoleculeExclusions);
	SequenceProcedureNode* forEachC = selectC_->addForEachBranch(ProcedureNode::AnalysisContext);
	forEachB->addNode(selectC_);

	// -- -- -- Calculate: 'rBC'
	CalculateDistanceProcedureNode* calcDistance = new CalculateDistanceProcedureNode(selectB_, selectC_);
	forEachC->addNode(calcDistance);

	// -- -- -- Calculate: 'aABC'
	CalculateAngleProcedureNode* calcAngle = new CalculateAngleProcedureNode(selectA_, selectB_, selectC_);
	forEachC->addNode(calcAngle);

	// -- -- -- Collect2D:  'Distance-Angle(B...C vs A-B...C)'
	collectDAngle_ = new Collect2DProcedureNode(calcDistance, calcAngle, 0.0, 10.0, 0.05, 0.0, 180.0, 1.0);
	SequenceProcedureNode* subCollection = collectDAngle_->addSubCollectBranch(ProcedureNode::AnalysisContext);
	forEachC->addNode(collectDAngle_);

	// -- -- -- -- Collect1D:  'RDF(BC)'
	collectDistance_ = new Collect1DProcedureNode(calcDistance, 0.0, 10.0, 0.05);
	subCollection->addNode(collectDistance_);

	// -- -- -- -- Collect1D:  'ANGLE(ABC)'
	collectAngle_ = new Collect1DProcedureNode(calcAngle, 0.0, 180.0, 1.0);
	subCollection->addNode(collectAngle_);

	// Process1D: 'RDF(BC)'
	processDistance_ = new Process1DProcedureNode(collectDistance_);
	processDistance_->setName("RDF(BC)");
	processDistance_->setKeyword<CharString>("LabelValue", "g(r)");
	processDistance_->setKeyword<CharString>("LabelX", "r, \\symbol{Angstrom}");

	SequenceProcedureNode* rdfNormalisation = processDistance_->addNormalisationBranch();
	RefList<SelectProcedureNode> sitePopulationNormalisers;
	sitePopulationNormalisers.append(selectA_);
	sitePopulationNormalisers.append(selectB_);
	rdfNormalisation->addNode(new OperateSitePopulationNormaliseProcedureNode(sitePopulationNormalisers));
	rdfNormalisation->addNode(new OperateNumberDensityNormaliseProcedureNode(selectC_));
	rdfNormalisation->addNode(new OperateSphericalShellNormaliseProcedureNode);
	analyser_.addRootSequenceNode(processDistance_);

	// Process1D: 'ANGLE(ABC)'
	processAngle_ = new Process1DProcedureNode(collectAngle_);
	processAngle_->setName("Angle(ABC)");
	processAngle_->setKeyword<CharString>("LabelValue", "Normalised Frequency");
	processAngle_->setKeyword<CharString>("LabelX", "\\symbol{theta}, \\symbol{degrees}");
	SequenceProcedureNode* angleNormalisation = processAngle_->addNormalisationBranch();
	angleNormalisation->addNode(new OperateNormaliseProcedureNode(1.0));
	analyser_.addRootSequenceNode(processAngle_);

	// Process2D: 'DAngle'
	processDAngle_ = new Process2DProcedureNode(collectDAngle_);
	processDAngle_->setName("DAngle");
	processDAngle_->setKeyword<CharString>("LabelValue", "g(r)");
	processDAngle_->setKeyword<CharString>("LabelX", "r, \\symbol{Angstrom}");
	processDAngle_->setKeyword<CharString>("LabelY", "\\symbol{theta}, \\symbol{degrees}");
	SequenceProcedureNode* dAngleNormalisation = processDAngle_->addNormalisationBranch();
	dAngleNormalisation->addNode(new OperateNormaliseProcedureNode(1.0));
	analyser_.addRootSequenceNode(processDAngle_);

	/*
	 * Keywords (including those exposed from the ProcedureNodes)
	 */

	// Calculation
	keywords_.add("Calculation", new Vec3DoubleKeyword(Vec3<double>(0.0, 10.0, 0.05), Vec3<double>(0.0, 0.0, 1.0e-5)), "DistanceRange", "Range (min, max, delta) of distance axis", "<min> <max> <delta> (Angstroms)");
	keywords_.add("Calculation", new Vec3DoubleKeyword(Vec3<double>(0.0, 180.0, 1.0), Vec3<double>(0.0, 0.0, 1.0e-5)), "AngleRange", "Range (min, max, delta) of angle axis", "<min> <max> <delta> (degrees)");

	// Sites
	keywords_.link("Sites", selectA_->keywords().find("Site"), "SiteA", "Add site(s) which represent 'A' in the interaction A-B...C", "<Species> <Site> [<Species> <Site> ... ]");
	keywords_.link("Sites", selectB_->keywords().find("Site"), "SiteB", "Add site(s) which represent 'B' in the interaction A-B...C", "<Species> <Site> [<Species> <Site> ... ]");
	keywords_.link("Sites", selectC_->keywords().find("Site"), "SiteC", "Add site(s) which represent 'C' in the interaction A-B...C", "<Species> <Site> [<Species> <Site> ... ]");
	keywords_.add("Sites", new BoolKeyword(false), "ExcludeSameMolecule", "Whether to exclude correlations between B and C sites on the same molecule", "<True|False>");

	// Export
	keywords_.link("Export", processDistance_->keywords().find("Save"), "SaveRDF", "Whether to save calculated B-C RDF to disk", "<True|False>");
	keywords_.link("Export", processAngle_->keywords().find("Save"), "SaveAngle", "Whether to save calculated A-B...C angle histrogram to disk", "<True|False>");
	keywords_.link("Export", processDAngle_->keywords().find("Save"), "SaveDAngle", "Whether to save calculated A-B...C angle map to disk", "<True|False>");
}

// Parse keyword line, returning true (1) on success, false (0) for recognised but failed, and -1 for not recognised
KeywordBase::ParseResult CalculateDAngleModule::parseComplexKeyword(KeywordBase* keyword, LineParser& parser, Dissolve* dissolve, GenericList& targetList, const char* prefix)
{
	return KeywordBase::Unrecognised;
}
