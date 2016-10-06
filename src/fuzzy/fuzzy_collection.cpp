#include "fuzzy_collection.h"

#include <iostream>

namespace fuzzy {

FuzzyCollection::FuzzyCollection(Domain domain)
    : domain_(domain) {

    input_ = (domain.end - domain.begin)/2;
}

void FuzzyCollection::addSet(FuzzySet set, std::string name) {
    sets_[name] = set;
}

Domain FuzzyCollection::getDomain() {
    return domain_;
}

FuzzySet FuzzyCollection::getSet(std::string name) {
    return sets_[name];
}

float FuzzyCollection::getValue(std::string set_name) {
    return sets_[set_name].getValue(input_);
}

void FuzzyCollection::setInput(float value) {
    input_ = value;
}

void FuzzyCollection::printValues() {
    for (std::map<std::string, FuzzySet>::iterator it=sets_.begin(); it!=sets_.end(); ++it) {
        std::cout << it->first << " = " << it->second.getValue(input_) << std::endl;
    }
}

} // namespace fuzzy
