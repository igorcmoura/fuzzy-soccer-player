#include "fuzzy_system.h"

#include <iostream>

#include "fuzzy.h"

namespace fuzzy {

FuzzySystem::FuzzySystem(Domain output_domain)
    : output_domain_(output_domain) {}

void FuzzySystem::addRule(FuzzyRule rule) {
    if (rule.getOutputDomain().equals(output_domain_))
        rules_.push_back(rule);
}

float FuzzySystem::getOutput(){
    return computeOutput();
}

float FuzzySystem::computeOutput() {
    FuzzySet output_set = createEmpty(output_domain_);
    for (FuzzyRule rule : rules_) {
        output_set = output_set.join(rule.getOutput());
    }
    return output_set.defuzzy();
}

} // namespace fuzzy
