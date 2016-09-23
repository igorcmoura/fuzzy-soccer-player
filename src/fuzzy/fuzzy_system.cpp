#include "fuzzy_system.h"

#include "fuzzy.h"

namespace fuzzy {

FuzzySystem::FuzzySystem() {}

void FuzzySystem::addRule(FuzzyRule rule) {
    rules_.push_back(rule);
}

void FuzzySystem::computeOutput(std::vector<float> positions) {
    FuzzySet ceslinhas = createEmpty(rules_.front().getOutputDomain()); //empty is a set of zeros
    for (FuzzyRule rule : rules_) {
        ceslinhas.join(rule.getOutput(positions));
    }

    value_ = ceslinhas.defuzzy();
}

float FuzzySystem::getOutput(std::vector<float> positions){
    computeOutput(positions);
    return value_;
}

} // namespace fuzzy