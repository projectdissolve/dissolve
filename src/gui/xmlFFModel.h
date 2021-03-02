// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include <QDialog>

#include <QAbstractTableModel>
#include <QModelIndex>

#include <tuple>
#include <vector>

class XmlFFModel : public QAbstractTableModel
{
    Q_OBJECT

    private:
    std::vector<std::tuple<std::string, std::string, double, double>> bonds_;

    public:
    XmlFFModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};
