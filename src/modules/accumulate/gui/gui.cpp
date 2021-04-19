// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#include "main/dissolve.h"
#include "modules/accumulate/accumulate.h"
#include "modules/accumulate/gui/modulewidget.h"

// Return a new widget controlling this Module
ModuleWidget *AccumulateModule::createWidget(QWidget *parent, Dissolve &dissolve)
{
    return new AccumulateModuleWidget(parent, dissolve.processingModuleData(), this, dissolve);
}
