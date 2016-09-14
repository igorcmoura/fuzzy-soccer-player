//
// Created by elRubo on 13/09/2016.
//

#include "FuzzyRule.h"

FuzzyRule::FuzzyRule(){}

void FuzzyRule::AddInput(FuzzySet input, float position){
    //receives input fuzzy sets and positions
    p_inputs.push_back(input);
    p_positions.push_back(position);
}

void FuzzyRule::setOutput(FuzzySet output){
    p_output = output;
}

FuzzySet FuzzyRule::getOutput(){
    computeAlpha();
    return p_output.cutAt(alpha);
}

void p_computeAlpha() {
    //find minimum of values
    min = p_inputs[0].getValue(p_positions[0]);
    for(int i =1; i<p_inputs.size(); i++){
        min = std::min(p_inputs[i].getValue(p_positions[i]), min);
    }
    alpha = min;
}