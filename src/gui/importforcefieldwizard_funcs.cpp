// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "gui/importforcefieldwizard.h"
#include <QFileDialog>
#include <pugixml.hpp>

ImportForcefieldWizard::ImportForcefieldWizard(QWidget *parent, Dissolve &mainDissolveInstance) : atoms_(mainDissolveInstance)
{
    ui_.setupUi(this);
    ui_.bondTable->setModel(&bonds_);
    ui_.angleTable->setModel(&angles_);
    ui_.atomTable->setModel(&atoms_);
    ui_.torsionTable->setModel(&torsions_);
    ui_.improperTable->setModel(&impropers_);

    updateNavButtons();
}

void ImportForcefieldWizard::updateNavButtons()
{
    int index = ui_.stackedWidget->currentIndex();
    int count = ui_.stackedWidget->count();

    ui_.backButton->setEnabled(index > 0);

    ui_.continueButton->setEnabled(index < count - 1 && atoms_.rowCount() > 0);
}

void ImportForcefieldWizard::nextStack()
{
    int index = ui_.stackedWidget->currentIndex();
    int count = ui_.stackedWidget->count();
    index += 1;
    ui_.stackedWidget->setCurrentIndex(index >= count ? count - 1 : index);

    updateNavButtons();
}

void ImportForcefieldWizard::prevStack()
{
    int index = ui_.stackedWidget->currentIndex();
    index -= 1;
    ui_.stackedWidget->setCurrentIndex(index < 0 ? 0 : index);

    updateNavButtons();
}

void ImportForcefieldWizard::xmlFileDialog()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Open xml file", "/home/adam", "OpenMM Xml files (*.xml)");
    if (fileName.isEmpty())
	return;

    ui_.lineEdit->setText(fileName);
}

void ImportForcefieldWizard::xmlString(QString fileName)
{
    pugi::xml_document doc;

    auto result = doc.load_file(fileName.toStdString().c_str());
    if (result)
    {
	auto root = doc.root();

	atoms_.readFile(root);
	bonds_.readFile(root);
	angles_.readFile(root);
	torsions_.readFile(root);
	impropers_.readFile(root);
	ui_.atomTable->resizeColumnsToContents();
	ui_.bondTable->resizeColumnsToContents();
	ui_.angleTable->resizeColumnsToContents();
	ui_.torsionTable->resizeColumnsToContents();
	ui_.improperTable->resizeColumnsToContents();
    }
    else
    {
	atoms_.clear();
    }
    updateNavButtons();
    return;
}
