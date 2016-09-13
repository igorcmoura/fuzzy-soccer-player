
#ifndef FUZZYSOCCERPLAYER_FUZZY_H
#define FUZZYSOCCERPLAYER_FUZZY_H

#include <vector>

#include "fuzzyset.h"

namespace fuzzy {

class Domain {
public:
    Domain(float begin, float end) : begin(begin), end(end) {};
    float begin, end;
};

FuzzySet createTrapezium(float a, float b, float c, float d, Domain domain);
FuzzySet createTriangle(float a, float b, float c, Domain domain);
FuzzySet createUpLine(float a, float b, Domain domain);
FuzzySet createDownLine(float a, float b, Domain domain);

std::vector<std::vector<float>> dot_product(std::vector<std::vector<float>> matrix_a, std::vector<std::vector<float>> matrix_b);

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZY_H
