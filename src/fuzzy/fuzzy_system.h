#ifndef FUZZYSOCCERPLAYER_FUZZYSYSTEM_H
#define FUZZYSOCCERPLAYER_FUZZYSYSTEM_H

#include <vector>

#include "fuzzy_rule.h"

namespace fuzzy {

class FuzzySystem {
public:
    FuzzySystem();

    void addRule(FuzzyRule rule);
    float getOutput();

private:
    void computeOutput();
    float value_;
    std::vector<FuzzyRule> rules_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYSYSTEM_H