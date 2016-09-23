#include "fuzzy_system.h"

#include "fuzzy.h"

namespace fuzzy {

FuzzySystem::FuzzySystem() {}

void FuzzySystem::addRule(FuzzyRule rule) {
    rules_.push_back(rule);
}

float FuzzySystem::computeOutput(std::vector<float> positions) {
    FuzzySet ouptup_set = createEmpty(rules_.front().getOutputDomain()); //empty is a set of zeros
    for (FuzzyRule rule : rules_) {
        ouptup_set = ouptup_set.join(rule.getOutput(positions));
    }
    return ouptup_set.defuzzy();
}

float FuzzySystem::getOutput(std::vector<float> positions){
    return computeOutput(positions);
}

} // namespace fuzzy