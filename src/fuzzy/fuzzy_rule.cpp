#include "fuzzy_rule.h"

#include <algorithm>

namespace fuzzy {

FuzzyRule::FuzzyRule(){}

void FuzzyRule::addInput(FuzzyCollection *collection, std::string fuzzy_set_name){
    inputs_.push_back(std::tuple<FuzzyCollection*, std::string>(collection, fuzzy_set_name));
}

void FuzzyRule::setOutput(FuzzyCollection *collection, std::string set_name){
    output_ = collection->getSet(set_name);
}

FuzzySet FuzzyRule::getOutput(){
    float alpha = computeAlpha();
    return output_.cutAt(alpha);
}

float FuzzyRule::computeAlpha() {
    //find minimum of values
    FuzzyCollection *collection = std::get<0>(inputs_[0]);
    std::string set_name = std::get<1>(inputs_[0]);
    float min = collection->getValue(set_name);

    for (int i = 1; i < inputs_.size(); i++) {
        collection = std::get<0>(inputs_[i]);
        set_name = std::get<1>(inputs_[i]);
        min = std::min(collection->getValue(set_name), min);
    }
    return min;
}

Domain FuzzyRule::getOutputDomain() {
    return output_.getDomain();
}

} // namespace fuzzy
