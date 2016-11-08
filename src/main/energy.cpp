/*
	*** dUQ - Energy
	*** src/main/energy.cpp
	Copyright T. Youngs 2012-2016

	This file is part of dUQ.

	dUQ is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	dUQ is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with dUQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main/duq.h"
#include "classes/grain.h"
#include "classes/box.h"
#include "classes/cell.h"
#include "classes/energykernel.h"
#include "classes/molecule.h"
#include "classes/species.h"
#include "base/processpool.h"
#include "base/timer.h"

// Return total intramolecular energy
double DUQ::intramolecularEnergy(ProcessPool& procPool, Configuration* cfg)
{
	/*
	 * Calculate the total intramolecular energy of the system, arising from Bond, Angle, and Torsion
	 * terms in all molecules.
	 * 
	 * This is a parallel routine, with processes operating as a standard world group.
	 */

	// Create an EnergyKernel
	EnergyKernel kernel(procPool, cfg, potentialMap_);

	double energy = 0.0;
	int start, stride;

	// Set start/skip for parallel loop
	start = procPool.interleavedLoopStart(ProcessPool::OverPoolProcesses);
	stride = procPool.interleavedLoopStride(ProcessPool::OverPoolProcesses);

	// Main loop over molecules
	for (int m=start; m<cfg->nMolecules(); m += stride)
	{
		Molecule* mol = cfg->molecule(m);

		// Bonds
		for (SpeciesBond* b = mol->species()->bonds(); b != NULL; b = b->next) energy += kernel.energy(mol, b);

		// Angles
		for (SpeciesAngle* a = mol->species()->angles(); a != NULL; a = a->next) energy += kernel.energy(mol, a);
	}

	Messenger::printVerbose("Intramolecular Energy (Local) is %15.9e\n", energy);
	
	// Sum energy and print
	procPool.allSum(&energy, 1);
	Messenger::printVerbose("Intramolecular Energy (World) is %15.9e\n", energy);

	return energy;
}

// Return total atom energy
double DUQ::interatomicEnergy(ProcessPool& procPool, Configuration* cfg)
{
	/*
	 * Calculates the total atom energy of the system, i.e. the energy contributions from PairPotential
	 * interactions between individual atoms.
	 * 
	 * This is a parallel routine, with processes operating as process groups.
	 */

	// Initialise the Cell distributor
	const bool willBeModified = false, allowRepeats = false;
	cfg->initialiseCellDistribution();

	// Create an EnergyKernel
	EnergyKernel kernel(procPool, cfg, potentialMap_);

	int cellId, n, m, start, stride;
	Cell* cell, *otherCell;
	double totalEnergy = 0.0;
/*	
	// TEST Invalidate all cell atom lists
	for (int n=0; n<cfg->nCells(); ++n)
	{
		cfg->cell(n)->atoms().invalidateLists();
		cfg->cell(n)->atomNeighbours().invalidateLists();
		cfg->cell(n)->mimAtomNeighbours().invalidateLists();
	}*/

	// Set start/skip for parallel loop
	start = procPool.interleavedLoopStart(ProcessPool::OverGroups);
	stride = procPool.interleavedLoopStride(ProcessPool::OverGroups);

	for (cellId = start; cellId<cfg->nCells(); cellId += stride)
	{
		cell = cfg->cell(cellId);

		/*
		 * Calculation Begins
		 */

		// This cell with itself
		totalEnergy += kernel.energy(cell, cell, false, true, ProcessPool::OverGroupProcesses);

		// Interatomic interactions between atoms in this cell and its neighbours
		totalEnergy += kernel.energy(cell, true, ProcessPool::OverGroupProcesses);

		/*
		 * Calculation End
		 */
	}

	// Print process-local energy
	Messenger::printVerbose("Atom Energy (Local) is %15.9e\n", totalEnergy);

	// Sum energy over all processes in the pool and print
	procPool.allSum(&totalEnergy, 1);
	Messenger::printVerbose("Atom Energy (World) is %15.9e\n", totalEnergy);

	return totalEnergy;
}

//Return Grain energy of the system
double DUQ::intergrainEnergy(ProcessPool& procPool, Configuration* cfg)
{
	/*
	 * Calculates the total Grain energy of the system, i.e. the energy contributions from PairPotential
	 * interactions between Grains.
	 * 
	 * This is a parallel routine, with processes operating as process groups.
	 */

	// Initialise the Cell distributor
	const bool willBeModified = false, allowRepeats = false;
	cfg->initialiseCellDistribution();

	// Create an EnergyKernel
	EnergyKernel kernel(procPool, cfg, potentialMap_);

	int cellId, n, m, start, stride;
	Cell* cell;
	Grain* grainI, *grainJ;
	double totalEnergy = 0.0;

	// Set start/skip for parallel loop
	start = procPool.interleavedLoopStart(ProcessPool::OverGroups);
	stride = procPool.interleavedLoopStride(ProcessPool::OverGroups);

	while (cellId = cfg->nextAvailableCell(procPool, willBeModified, allowRepeats), cellId != Cell::AllCellsComplete)
	{
		// Check for valid cell
		if (cellId == Cell::NoCellsAvailable)
		{
			Messenger::printVerbose("Nothing for this process to do.\n");
			cfg->finishedWithCell(procPool, willBeModified, cellId);
			continue;
		}
		cell = cfg->cell(cellId);
		Messenger::printVerbose("Cell %i now the target, containing %i Grains interacting with %i neighbours.\n", cellId, cell->nGrains(), cell->nTotalCellNeighbours());

		/*
		 * Calculation Begins
		 */
		
		// Loop over Grains
		// Parallelism employed in the outer loop - as such, ProcessPool::Individual is passed to the Grain/Cell method.
		for (n=start; n<cell->nGrains(); n += stride)
		{
			grainI = cell->grain(n);
			totalEnergy += kernel.energy(grainI, true, ProcessPool::Individual);

// 			// Inter-Grain interactions in this Cell...
			// TODO??
// 			for (m=n+1; m<cell->nGrains(); ++m)
// 			{
// 				grainJ = cell->grain(m);
// 				totalEnergy += kernel.energy(grainI, grainJ, true, false);
// 			}
// 			
// 			// Inter-Grain interactions between this Grain and those in Cell neighbours
// 			totalEnergy += kernel.energy(grainI, cell->atomNeighbours(), false, true, ProcessPool::Solo);
// 			totalEnergy += kernel.energy(grainI, cell->mimAtomNeighbours(), true, true, ProcessPool::Solo);
		}

		/*
		 * Calculation End
		 */
		// Must unlock the Cell when we are done with it!
		cfg->finishedWithCell(procPool, willBeModified, cellId);
	}
	Messenger::printVerbose("Grain Energy (Local) is %15.9e\n", totalEnergy);
	
	// Sum energy and print
	procPool.allSum(&totalEnergy, 1);
	Messenger::printVerbose("Grain Energy (World) is %15.9e\n", totalEnergy);

	return totalEnergy;
}

// Test - Return total energy of the system using Molecules
double DUQ::totalEnergyTestMolecules(Configuration* cfg)
{
	/*
	 * Calculate the total energy of the system using a loop over Molecules.
	 * 
	 * This is a serial routine, with each process calculating its own value.
	 */

	// Loop over Molecules
	// We will end up double-counting all Molecule-Molecule interactions, but only single-counting intramolecular terms.
	// So, scale PairPotential contributions to the Grain energy, but leave inter-Grain correction energies as-is.
	double totalEnergy = 0.0;
// 	for (Molecule* mol = cfg->molecules(); mol != NULL; mol = mol->next) totalEnergy += kernel.energy(mol, ProcessPool::Solo, 0.5, 1.0, 1.0);
	Messenger::print("DUQ::totalEnergyTestMolecules IS HORRIBLY BROKEN.\n");
	return totalEnergy;
}
