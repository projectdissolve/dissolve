/*
	*** Reference Point
	*** src/gui/referencepoint.cpp
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

#include "gui/referencepoint.h"

// Constructor / Destructor
ReferencePoint::ReferencePoint() : ListItem<ReferencePoint>()
{
}

ReferencePoint::~ReferencePoint()
{
}

// Set suffix for data items
void ReferencePoint::setSuffix(QString suffix)
{
	suffix_ = suffix;
}

// Return suffix for data items
QString ReferencePoint::suffix() const
{
	return suffix_;
}

// Set restart file from which the reference point data was read
void ReferencePoint::setRestartFile(QString restartFile)
{
	restartFile_ = restartFile;
}

// Return restart file from which the reference point data was read
QString ReferencePoint::restartFile() const
{
	return restartFile_;
}
