#ifndef FUZZYSOCCERPLAYER_FUZZYRULE_H
#define FUZZYSOCCERPLAYER_FUZZYRULE_H

#include <string>
#include <tuple>
#include <vector>

#include "fuzzy_set.h"
#include "fuzzy_collection.h"

namespace fuzzy {

class FuzzyRule {
public:
    FuzzyRule();

    void addInput(FuzzyCollection *collection, std::string fuzzy_set_name);
    void setOutput(FuzzyCollection *collection, std::string set_name);
    FuzzySet getOutput();
    Domain getOutputDomain();

private:
    std::vector<std::tuple<FuzzyCollection*, std::string>> inputs_;
    FuzzySet output_;

    float computeAlpha();

};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYRULE_H
