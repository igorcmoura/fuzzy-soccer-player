#include "fuzzy_rule.h"

namespace fuzzy {

FuzzyRule::FuzzyRule(){}

void FuzzyRule::addInput(FuzzySet input){
    //receives input fuzzy sets and positions
    p_inputs.push_back(input);
}

void FuzzyRule::setOutput(FuzzySet output){
    p_output = output;
}

FuzzySet FuzzyRule::getOutput(std::vector<float> positions){
    float alpha = computeAlpha(positions);
    return p_output.cutAt(alpha);
}

float FuzzyRule::computeAlpha(std::vector<float> positions) {
    //find minimum of values
    float min = p_inputs[0].getValue(positions[0]);
    for (int i = 1; i < p_inputs.size(); i++) {
        min = std::min(p_inputs[i].getValue(positions[i]), min);
    }
    return min;
}

Domain FuzzyRule::getOutputDomain() {
    return p_output.getDomain();
}

} // namespace fuzzy
