// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "math/integrator.h"
#include "math/data1d.h"
#include "math/data2d.h"
#include "math/data3d.h"
#include "templates/array.h"
#include <numeric>

/*
 * Static Functions
 */

// Compute integral of supplied data via trapezoid rule
double Integrator::trapezoid(const Data1D &data)
{
    // Check for insufficient data
    if (data.nValues() < 2)
        return 0.0;

    // Grab data arrays
    const auto &x = data.xAxis();
    const auto &y = data.values();

    double total = 0.0, y0 = y.front(), y1, x0 = x.front(), x1;
    for (auto n = 1; n < x.size(); ++n)
    {
        x1 = x[n];
        y1 = y[n];
        total += (x1 - x0) * (y0 + y1) * 0.5;
        x0 = x1;
        y0 = y1;
    }
    return total;
}

// Compute integral of supplied data via trapezoid rule between the specified limits
double Integrator::trapezoid(const Data1D &data, double xMin, double xMax)
{
    // Check for insufficient data
    if (data.nValues() < 2)
        return 0.0;

    // Grab data arrays
    const auto &x = data.xAxis();
    const auto &y = data.values();

    double total = 0.0, y0, y1, x0, x1;
    auto nPoints = 0;
    for (auto n = 0; n < x.size(); ++n)
    {
        // Get current x and y values and check limit
        x1 = x[n];
        y1 = y[n];
        if (x1 < xMin)
            continue;
        if (x1 > xMax)
            break;

        // If this is the first point, don't try to increase integral
        if (nPoints > 0)
            total += (x1 - x0) * (y0 + y1) * 0.5;

        // Store old values
        x0 = x1;
        y0 = y1;

        ++nPoints;
    }

    return total;
}

// Compute integral of supplied data via trapezoid rule within the specified range
double Integrator::trapezoid(const Data1D &data, const Range range)
{
    return trapezoid(data, range.minimum(), range.maximum());
}

// Compute absolute integral of supplied data via trapezoid rule
double Integrator::absTrapezoid(const Data1D &data)
{
    // Check for insufficient data
    if (data.nValues() < 2)
        return 0.0;

    // Grab data arrays
    const auto &x = data.xAxis();
    const auto &y = data.values();

    double total = 0.0, y0 = y.front(), y1, x0 = x.front(), x1;
    for (auto n = 1; n < x.size(); ++n)
    {
        x1 = x[n];
        y1 = y[n];
        total += fabs((x1 - x0) * (y0 + y1) * 0.5);
        x0 = x1;
        y0 = y1;
    }
    return total;
}

// Return sum of all values in supplied data
double Integrator::sum(const Data1D &data)
{
    // Grab data array
    const auto &values = data.values();

    double total = 0.0;

    for (auto n = 0; n < values.size(); ++n)
        total += values[n];

    return total;
}

// Return sum of supplied data between the specified limits
double Integrator::sum(const Data1D &data, double xMin, double xMax)
{
    // Grab data arrays
    const auto &x = data.xAxis();
    const auto &values = data.values();

    double total = 0.0;

    for (auto n = 0; n < values.size(); ++n)
    {
        if (x[n] < xMin)
            continue;
        if (x[n] > xMax)
            break;

        total += values[n];
    }

    return total;
}

// Return sum of supplied data within the specified range
double Integrator::sum(const Data1D &data, const Range range) { return sum(data, range.minimum(), range.maximum()); }

// Return sum of all absolute values of supplied data
double Integrator::absSum(const Data1D &data)
{
    // Grab data array
    const auto &values = data.values();

    double total = 0.0;

    for (auto n = 0; n < values.size(); ++n)
        total += fabs(values[n]);

    return total;
}

// Return sum of all absolute values between the specified limits
double Integrator::absSum(const Data1D &data, double xMin, double xMax)
{
    // Grab data arrays
    const auto &x = data.xAxis();
    const auto &values = data.values();

    double total = 0.0;

    for (auto n = 0; n < values.size(); ++n)
    {
        if (x[n] < xMin)
            continue;
        if (x[n] > xMax)
            break;

        total += fabs(values[n]);
    }

    return total;
}

// Return sum of all absolute values within the specified range
double Integrator::absSum(const Data1D &data, const Range range) { return absSum(data, range.minimum(), range.maximum()); }

// Return sum of squares of all values in supplied data
double Integrator::sumOfSquares(const Data1D &data)
{
    // Grab data array
    const auto &values = data.values();

    double total = 0.0;

    for (auto n = 0; n < values.size(); ++n)
        total += values[n] * values[n];

    return total;
}

// Return sum of squares of values between the specified limits
double Integrator::sumOfSquares(const Data1D &data, double xMin, double xMax)
{
    // Grab data arrays
    const auto &x = data.xAxis();
    const auto &values = data.values();

    double total = 0.0;

    for (auto n = 0; n < values.size(); ++n)
    {
        if (x[n] < xMin)
            continue;
        if (x[n] > xMax)
            break;

        total += values[n] * values[n];
    }

    return total;
}

// Return sum of squares of values within the specified range
double Integrator::sumOfSquares(const Data1D &data, const Range range)
{
    return sumOfSquares(data, range.minimum(), range.maximum());
}

// Return sum of all values in supplied data
double Integrator::sum(const Data2D &data)
{
    return std::accumulate(data.constValues2D().begin(), data.constValues2D().end(), 0.0);
}

// Return sum of all absolute values in supplied data
double Integrator::absSum(const Data2D &data)
{
    return std::accumulate(data.constValues2D().begin(), data.constValues2D().end(), 0.0,
                           [](auto a, auto b) { return fabs(a) + fabs(b); });
}

// Return sum of all values in supplied data
double Integrator::sum(const Data3D &data)
{
    // Grab data array
    const Array3D<double> &values = data.constValues3D();

    return std::accumulate(values.linearArray().begin(), values.linearArray().end(), 0.0);
}

// Return sum of all absolute values in supplied data
double Integrator::absSum(const Data3D &data)
{
    // Grab data array
    const Array3D<double> &values = data.constValues3D();

    return std::accumulate(values.linearArray().begin(), values.linearArray().end(), 0.0,
                           [](auto acc, auto n) { return acc + fabs(n); });
}
