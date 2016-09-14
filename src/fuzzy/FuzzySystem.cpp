//
// Created by elRubo on 13/09/2016.
//

#include "FuzzySystem.h"

namespace fuzzy {

    FuzzySystem::FuzzySystem() {}

    void FuzzySystem::addRule(FuzzyRule rule) {
        rules_.push_back(rule);
    }

    void FuzzySystem::computeOutput() {
        FuzzySet ceslinhas = FuzzySet::empty(); //empty is a set of zeros
        for (FuzzyRule rule : rules_) {
            ceslinhas.join(rule.getOutput());
        }

        value_ = ceslinhas.defuzzy();
    }

    float FuzzySystem::FuzzySystem::getOutput(){
        computeOutput();
        return value_;
    }

} // namespace fuzzy