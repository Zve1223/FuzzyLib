#include <iostream>
#include <unordered_map>
#include <unordered_set>

typedef char data_t;
typedef std::unordered_map<data_t, double> fuzzy_map;
typedef std::unordered_set<data_t>         data_set;

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

struct FuzzySet {
public:
    FuzzySet() = default;
    FuzzySet(size_t n) : data(n) {}
    ~FuzzySet() = default;

    inline void add(const data_t member, double membership) noexcept {
        data[member] = membership;
    }

    double *get(const data_t member) noexcept {
        auto temp = data.find(member);
        if (temp == data.end()) {
            return nullptr;
        } else {
            return &temp->second;
        }
    }

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

struct FuzzyRelation {
public:
    FuzzyRelation(const FuzzySet &a, const FuzzySet &b) : a(ExtractKeys(a)), b(ExtractKeys(b)) {
        size_t x = 0, y = 0;
        relation = new double*[a.size()];
        for (const auto& elementA : a) {
            double *temp = new double[b.size()];
            for (const auto& elementB : b) {
                temp[x++] = std::min(elementA.second, elementB.second);
            }
            x = 0;
            relation[y++] = temp;
        }
    }
    FuzzyRelation(const data_set &a, const data_set &b, double **relation) : a(a), b(b), relation(relation) {}

    ~FuzzyRelation() noexcept {
        for (size_t y = 0; y < a.size(); y++) {
            delete[] relation[y];
        }
        delete[] relation;
    }

    double get(const data_t keyA, const data_t keyB) const noexcept {
        auto valueA = a.find(keyA);
        if (valueA == a.end()) {
            return 0.0;
        }
        auto valueB = b.find(keyB);
        if (valueB == b.end()) {
            return 0.0;
        }
        return relation[std::distance(a.begin(), valueA)][std::distance(b.begin(), valueB)];
    }

    const data_set a;
    const data_set b;

private:
    double **relation;
};

FuzzyRelation Intersection(const FuzzyRelation &r1, const FuzzyRelation &r2) {
    data_set a;
    for (const auto& keyA : r1.a) {
        if (r2.a.find(keyA) != r2.a.end()) {
            a.insert(keyA);
        }
    }

    data_set b;
    for (const auto& keyB : r2.b) {
        if (r1.b.find(keyB) != r1.b.end()) {
            b.insert(keyB);
        }
    }

    double **result = new double *[a.size()];
    size_t x = 0, y = 0;
    for (const auto& keyA : a) {
        double *temp = new double[b.size()];
        for (const auto& keyB : b) {
            temp[x++] = std::min(r1.get(keyA, keyB), r2.get(keyA, keyB));
        }
        x = 0;
        result[y++] = temp;
    }

    return FuzzyRelation(a, b, result);
}

FuzzyRelation Union(const FuzzyRelation &r1, const FuzzyRelation &r2) {
    data_set a = r1.a;
    for (const auto& keyB : r2.a) {
        if (r2.a.find(keyB) != r2.a.end()) {
            a.insert(keyB);
        }
    }

    data_set b = r1.b;
    for (const auto& keyA : r2.b) {
        if (r2.b.find(keyA) != r2.b.end()) {
            b.insert(keyA);
        }
    }

    size_t x = 0, y = 0;
    double **result = new double *[a.size()];
    for (const auto& keyA : a) {
        double *temp = new double[b.size()];
        for (const auto& keyB : b) {
            temp[x++] = std::max(r1.get(keyA, keyB), r2.get(keyA, keyB));
        }
        x = 0;
        result[y++] = temp;
    }

    return FuzzyRelation(a, b, result);
}

FuzzyRelation Complement(const FuzzyRelation &r) {
    size_t x = 0, y = 0;
    double **result = new double *[r.a.size()];
    for (const auto& keyA : r.a) {
        double *temp = new double[r.b.size()];
        for (const auto& keyB : r.b) {
            temp[x++] = 1.0 - r.get(keyA, keyB);
        }
        x = 0;
        result[y++] = temp;
    }
    return FuzzyRelation(r.a, r.b, result);
}

FuzzyRelation Composition(const FuzzyRelation &r1, const FuzzyRelation &r2) {
    double **result = new double *[r1.a.size()];
    for (size_t y = 0; y < r1.a.size(); y++) {
        result[y] = new double[r2.b.size()];
    }

    data_set broker = Union(r1.b, r2.a);
    
    double *tempA = new double[r1.a.size()];
    size_t y = 0;
    for (const auto& keyA : r1.a) {
        double minValue = DBL_MAX;
        for (const auto& keyB : broker) {
            minValue = std::min(minValue, r1.get(keyA, keyB));
        }
        tempA[y++] = ((minValue == DBL_MAX) ? 0.0 : minValue);
    }
    
    double *tempB = new double[r2.b.size()];
    size_t x = 0;
    for (const auto& keyB : r2.b) {
        double minValue = DBL_MAX;
        for (const auto& keyA : broker) {
            minValue = std::min(minValue, r2.get(keyA, keyB));
        }
        tempB[x++] = ((minValue == DBL_MAX) ? 0.0 : minValue);
    }

    for (size_t y = 0; y < r1.a.size(); y++) {
        for (size_t x = 0; x < r2.b.size(); x++) {
            result[y][x] = std::max(tempA[y], tempB[x]);
        }
    }

    return FuzzyRelation(r1.a, r2.b, result);
}

void PrintRelation(const FuzzyRelation &r) {
    for (size_t i = 0; i < (r.b.size() + 1) * 8 + 4; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;

    std::cout << '|' << ' ';
    for (const auto& keyB : r.b) {
        std::cout << '\t' << ' ' << keyB;
    }
    std::cout << '\t' << ' ' << ' ' << ' ' << '|' << std::endl;
    
    for (const auto& keyA : r.a) {
        std::cout << '|' << ' ' << ' ' << ' ';
        std::cout << keyA;
        for (const auto& keyB : r.b) {
            std::cout << '\t' << r.get(keyA, keyB);
        }
        std::cout << '\t' << ' ' << ' ' << ' ' << '|' << std::endl;
    }

    for (size_t i = 0; i < (r.b.size() + 1) * 8 + 4; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

int main() {
    FuzzySet A;
    A.add('a', 0.2);
    A.add('b', 0.7);
    A.add('c', 0.4);
    
    FuzzySet B;
    B.add('d', 0.5);
    B.add('e', 0.6);

    FuzzySet C;
    C.add('a', 0.6);
    C.add('c', 0.1);
    C.add('e', 0.1);

    FuzzySet D;
    D.add('d', 0.9);
    D.add('e', 0.2);
    D.add('f', 0.2);

    FuzzyRelation ARB(A, B);

    FuzzyRelation CRD(C, D);

    std::cout << std::endl << "ARB" << std::endl;
    PrintRelation(ARB);

    std::cout << std::endl << "CARB" << std::endl;
    PrintRelation(Complement(ARB));

    std::cout << std::endl << "CRD" << std::endl;
    PrintRelation(CRD);

    std::cout << std::endl << "CCRD" << std::endl;
    PrintRelation(Complement(CRD));

    std::cout << std::endl << "ARBICRD" << std::endl;
    PrintRelation(Intersection(ARB, CRD));

    std::cout << std::endl << "ARBUCRD" << std::endl;
    PrintRelation(Union(ARB, CRD));

    std::cout << std::endl << "ARBCCRD" << std::endl;
    PrintRelation(Union(ARB, CRD));

    return 0;
}
