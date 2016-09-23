#ifndef FUZZYSOCCERPLAYER_FUZZYSYSTEM_H
#define FUZZYSOCCERPLAYER_FUZZYSYSTEM_H

#include <vector>

#include "fuzzy_rule.h"

namespace fuzzy {

class FuzzySystem {
public:
    FuzzySystem();

    void addRule(FuzzyRule rule);
    float getOutput(std::vector<float> positions);

private:
    float computeOutput(std::vector<float> positions);
    std::vector<FuzzyRule> rules_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYSYSTEM_H