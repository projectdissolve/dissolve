// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2020 Team Dissolve and contributors

#include "expression/binary.h"

ExpressionBinaryOperatorNode::ExpressionBinaryOperatorNode(BinaryOperator op) : ExpressionNodeNEW(), operator_(op) {}

/*
 * Evaluation
 */

// Evaluate node
std::optional<ExpressionValue> ExpressionBinaryOperatorNode::evaluate() const
{
    // Must be two child nodes
    if (children_.size() != 2)
        return std::nullopt;

    // Evaluate LHS and RHS nodes
    auto optl = children_[0]->evaluate();
    if (!optl)
        return std::nullopt;
    auto optr = children_[1]->evaluate();
    if (!optr)
        return std::nullopt;

    // Perform the operation
    ExpressionValue result;
    auto lhs = (*optl), rhs = (*optr);
    switch (operator_)
    {
        case (OperatorAdd):
            if (ExpressionValue::bothIntegers(lhs, rhs))
                result = lhs.asInteger() + rhs.asInteger();
            else
                result = lhs.asDouble() + rhs.asDouble();
            break;
        case (OperatorDivide):
            if (ExpressionValue::bothIntegers(lhs, rhs))
                result = lhs.asInteger() / rhs.asInteger();
            else
                result = lhs.asDouble() / rhs.asDouble();
            break;
        case (OperatorSubtract):
            if (ExpressionValue::bothIntegers(lhs, rhs))
                result = lhs.asInteger() - rhs.asInteger();
            else
                result = lhs.asDouble() - rhs.asDouble();
            break;
        case (OperatorPow):
            result = pow(lhs.asDouble(), rhs.asDouble());
            break;
        case (OperatorMultiply):
            if (ExpressionValue::bothIntegers(lhs, rhs))
                result = lhs.asInteger() * rhs.asInteger();
            else
                result = lhs.asDouble() * rhs.asDouble();
            break;
    }

    return result;
}