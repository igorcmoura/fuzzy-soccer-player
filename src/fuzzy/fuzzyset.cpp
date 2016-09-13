
#include "fuzzyset.h"

namespace fuzzy {

FuzzySet::FuzzySet(float low_1, float high_1, float high_2, float low_2, Domain domain)
    : low_1_(low_1), low_2_(low_2), high_1_(high_1), high_2_(high_2) {}

float FuzzySet::fuzzifie(float value) {
    if (value >= high_1_ and value <= high_2_)
        return 1.0;
    else if (value > low_1_ and value < high_1_)
        return (value - low_1_)/(high_1_ - low_1_);
    else if (value > high_2_ and value < low_2_)
        return (low_2_ - value)/(low_2_ - high_2_);
    else
        return 0.0;
}

}
