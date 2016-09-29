#include "fuzzy_collection.h"

namespace fuzzy {

FuzzyCollection::FuzzyCollection(Domain domain) {
    input_ = (domain.end - domain.begin)/2;
}

void FuzzyCollection::addSet(FuzzySet set, std::string name) {
    sets_[name] = set;
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

} // namespace fuzzy
