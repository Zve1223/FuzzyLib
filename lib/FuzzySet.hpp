#pragma once

#include <unordered_map>
#include <unordered_set>

namespace FuzzyLib {

typedef char data_t;
typedef std::unordered_map<data_t, double> fuzzy_map;
typedef std::unordered_set<data_t>         data_set;

data_set Union(const data_set &a, const data_set &b);
data_set Intersection(const data_set &a, const data_set &b);

struct FuzzySet {
public:
    FuzzySet() = default;
    FuzzySet(size_t n) : data(n) {}
    ~FuzzySet() = default;

    inline void add(const data_t member, double membership) noexcept {
        data[member] = membership;
    }

    double *get(const data_t member) noexcept;

    inline auto begin() const noexcept {
        return data.begin();
    }

    inline auto find(const data_t member) const noexcept {
        return data.find(member);
    }

    inline auto end() const noexcept {
        return data.end();
    }

    inline double &operator[](const data_t key) {
        return data[key];
    }

    inline const double &operator[](const data_t key) const {
        return data.at(key);
    }
    
    inline size_t size() const noexcept {
        return data.size();
    }

private:
    fuzzy_map data;
};

FuzzySet Intersection(const FuzzySet &a, const FuzzySet &b);

FuzzySet Union(const FuzzySet &a, const FuzzySet &b);

FuzzySet Complement(const FuzzySet &set);

data_set ExtractKeys(const FuzzySet &set);

} // namespace FuzzyLib