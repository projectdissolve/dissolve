/*
	*** Data Manager Dialog
	*** src/gui/datamanagerdialog.h
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

#ifndef DISSOLVE_DIALOG_DATAMANAGER_H
#define DISSOLVE_DIALOG_DATAMANAGER_H

#include "gui/ui_datamanagerdialog.h"
#include "templates/list.h"
#include <QDialog>

// Forward Declarations
class Dissolve;
class GenericItem;

// Data Manager Dialog
class DataManagerDialog : public QDialog
{
	Q_OBJECT

	public:
	// Constructor
	DataManagerDialog(QWidget* parent, Dissolve& dissolve);
	// Destructor
	~DataManagerDialog();


	/*
	 * UI
	 */
	private:
	// Main form declaration
	Ui::DataManagerDialog ui_;

	private:
	// Append GenericItems to table under specified source
	void addItemsToTable(QTableWidget* table, List<GenericItem>& items, const char* locationName, const char* locationIconResource);
	// Update the specified table of GenericItems, optionally filtering them by name and description
	void filterTable(QTableWidget* table, GenericItem* current, QString filter);

	private slots:
	// Simulation Data
	void on_SimulationDataFilterEdit_textChanged(const QString& text);

	// Dialog
	void on_CloseButton_clicked(bool checked);
};

#endif
