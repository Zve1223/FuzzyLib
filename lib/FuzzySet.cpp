#include "FuzzySet.hpp"

using namespace FuzzyLib;

data_set Union(const data_set &a, const data_set &b) {
    data_set result = a;
    for (const auto& elementB : b) {
        if (a.find(elementB) == a.end()) {
            result.insert(elementB);
        }
    }
    return result;
}

data_set Intersection(const data_set &a, const data_set &b) {
    data_set result;
    for (const auto& elementA : a) {
        if (b.find(elementA) != b.end()) {
            result.insert(elementA);
        }
    }
    return result;
}

double * FuzzySet::get(const data_t member) noexcept {
    auto temp = data.find(member);
    if (temp == data.end()) {
        return nullptr;
    } else {
        return &temp->second;
    }
}

FuzzySet Intersection(const FuzzySet &a, const FuzzySet &b) {
    FuzzySet result;
    for (const auto& elementA : a) {
        if (b.find(elementA.first) == b.end()) {
            continue;
        }
        result.add(elementA.first, std::min(elementA.second, b[elementA.first]));
    }
    return result;
}

FuzzySet Union(const FuzzySet &a, const FuzzySet &b) {
    FuzzySet result;
    for (const auto& elementA : a) {
        if (b.find(elementA.first) == b.end()) {
            result.add(elementA.first, elementA.second);
        } else {
            result.add(elementA.first, std::max(elementA.second, b[elementA.first]));
        }
    }
    for (const auto& elementB : b) {
        if (a.find(elementB.first) == a.end()) {
            result.add(elementB.first, elementB.second);
        }
    }
    return result;
}

FuzzySet Complement(const FuzzySet &set) {
    FuzzySet result;
    for (const auto& element : set) {
        result[element.first] = 1.0 - element.second;
    }
    return result;
}

data_set ExtractKeys(const FuzzySet &set) {
    data_set result;
    result.reserve(set.size());
    for (const auto& element : set) {
        result.insert(element.first);
    }
    return result;
}