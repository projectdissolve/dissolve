/*
	*** ProcessingTab Functions
	*** src/gui/processingtab_funcs.cpp
	Copyright T. Youngs 2012-2018

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

#include "gui/processingtab.h"
#include "main/duq.h"
#include "classes/configuration.h"
#include "base/lineparser.h"

// Constructor / Destructor
ProcessingTab::ProcessingTab(DUQWindow* duqWindow, DUQ& duq, QTabWidget* parent, const char* title) : MainTab(duqWindow, duq, parent, CharString("Processing", title), this)
{
	ui.setupUi(this);

	addModuleWidgets(duq_.processingModules().modules(), subWidgets_, ui.ModuleWidgetLayout);
}

ProcessingTab::~ProcessingTab()
{
}

/*
 * SubWidget / SubWindow Handling
 */

// Return the tab's SubWindow area, if it has one
QMdiArea* ProcessingTab::subWindowArea()
{
	return NULL;
}

// Return the tab's SubWidget layout, if it has one
QLayout* ProcessingTab::subWidgetLayout()
{
	return ui.ModuleWidgetLayout;
}

/*
 * Update
 */

// Update controls in tab
void ProcessingTab::updateControls()
{
	// Loop over our SubWidgets
	ListIterator<SubWidget> subWidgetIterator(subWidgets_);
	while (SubWidget* subWidget = subWidgetIterator.iterate()) subWidget->updateControls();
}

// Disable sensitive controls within tab, ready for main code to run
void ProcessingTab::disableSensitiveControls()
{
	// Disable sensitive controls in SubWidgets
	ListIterator<SubWidget> subWidgetIterator(subWidgets_);
	while (SubWidget* subWidget = subWidgetIterator.iterate()) subWidget->disableSensitiveControls();
}

// Enable sensitive controls within tab, ready for main code to run
void ProcessingTab::enableSensitiveControls()
{
	// Enable sensitive controls in SubWidgets
	ListIterator<SubWidget> subWidgetIterator(subWidgets_);
	while (SubWidget* subWidget = subWidgetIterator.iterate()) subWidget->enableSensitiveControls();
}

/*
 * State
 */

// Write widget state through specified LineParser
bool ProcessingTab::writeState(LineParser& parser)
{
	// Loop over our SubWidgets
	ListIterator<SubWidget> subWidgetIterator(subWidgets_);
	while (SubWidget* subWidget = subWidgetIterator.iterate())
	{
		// Write window state
		if (!parser.writeLineF("'%s'  %s  '%s'\n", title_.get(), subWidget->widgetType(), subWidget->title())) return false;
		if (!parser.writeLineF("0\n")) return false;
		if (!subWidget->writeState(parser)) return false;
	}

	return true;
}
