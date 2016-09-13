
#ifndef FUZZYSOCCERPLAYER_FUZZYSET_H
#define FUZZYSOCCERPLAYER_FUZZYSET_H

#include "fuzzy.h"

namespace fuzzy {

class FuzzySet {
public:
    FuzzySet(float low_1, float high_1, float high_2, float low_2, Domain domain);

    float fuzzifie(float value);

private:
    float low_1_, high_1_, high_2_, low_2_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYSET_H
