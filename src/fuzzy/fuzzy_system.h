#ifndef FUZZYSOCCERPLAYER_FUZZYSYSTEM_H
#define FUZZYSOCCERPLAYER_FUZZYSYSTEM_H

#include <vector>

#include "fuzzy_rule.h"

namespace fuzzy {

class FuzzySystem {
public:
    FuzzySystem(Domain output_domain);

    void addRule(FuzzyRule rule);
    float getOutput();

private:
    float computeOutput();
    std::vector<FuzzyRule> rules_;

    Domain output_domain_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYSYSTEM_H
