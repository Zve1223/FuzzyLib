#pragma once

#include <iostream>

#include "FuzzySet.hpp"

namespace FuzzyLib {

struct FuzzyRelation {
public:
    FuzzyRelation(const FuzzySet &a, const FuzzySet &b);
    FuzzyRelation(const data_set &a, const data_set &b, double **relation);
    ~FuzzyRelation() noexcept;

    double get(const data_t keyA, const data_t keyB) const noexcept;

    const data_set a;
    const data_set b;

private:
    double **relation;
};

FuzzyRelation Intersection(const FuzzyRelation &r1, const FuzzyRelation &r2);
FuzzyRelation Union(const FuzzyRelation &r1, const FuzzyRelation &r2);
FuzzyRelation Complement(const FuzzyRelation &r);
FuzzyRelation Composition(const FuzzyRelation &r1, const FuzzyRelation &r2);
void PrintRelation(const FuzzyRelation &r);

} // namespace FuzzyLib
