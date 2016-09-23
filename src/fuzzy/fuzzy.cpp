#include <iostream>
#include "fuzzy.h"

namespace fuzzy {

FuzzySet createTrapezium(float a, float b, float c, float d, Domain domain) {
    FuzzySet fuzzy_set = FuzzySet();
    fuzzy_set.addPoint(domain.begin, 0.0);
    fuzzy_set.addPoint(a, 0.0);
    fuzzy_set.addPoint(b, 1.0);
    fuzzy_set.addPoint(c, 1.0);
    fuzzy_set.addPoint(d, 0.0);
    fuzzy_set.addPoint(domain.end, 0.0);
    return fuzzy_set;
}

FuzzySet createTriangle(float a, float b, float c, Domain domain) {
    FuzzySet fuzzy_set = FuzzySet();
    fuzzy_set.addPoint(domain.begin, 0.0);
    fuzzy_set.addPoint(a, 0.0);
    fuzzy_set.addPoint(b, 1.0);
    fuzzy_set.addPoint(c, 0.0);
    fuzzy_set.addPoint(domain.end, 0.0);
    return fuzzy_set;
}

FuzzySet createUpLine(float a, float b, Domain domain) {
    FuzzySet fuzzy_set = FuzzySet();
    fuzzy_set.addPoint(domain.begin, 0.0);
    fuzzy_set.addPoint(a, 0.0);
    fuzzy_set.addPoint(b, 1.0);
    fuzzy_set.addPoint(domain.end, 1.0);
    return fuzzy_set;
}

FuzzySet createDownLine(float a, float b, Domain domain) {
    FuzzySet fuzzy_set = FuzzySet();
    fuzzy_set.addPoint(domain.begin, 1.0);
    fuzzy_set.addPoint(a, 1.0);
    fuzzy_set.addPoint(b, 0.0);
    fuzzy_set.addPoint(domain.end, 0.0);
    return fuzzy_set;
}
FuzzySet createEmpty(Domain domain) {
     FuzzySet fuzzy_set = FuzzySet();
     fuzzy_set.addPoint(domain.begin, 0.0);
     fuzzy_set.addPoint(domain.end, 0.0);
     return fuzzy_set;
}

} // namespace fuzzy
