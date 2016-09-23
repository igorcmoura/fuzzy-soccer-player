#ifndef FUZZYSOCCERPLAYER_FUZZYRULE_H
#define FUZZYSOCCERPLAYER_FUZZYRULE_H

#include<vector>
#include<algorithm> //for std::min

#include "fuzzy_set.h"

namespace fuzzy {

class FuzzyRule {
public:
    FuzzyRule();

    void addInput(FuzzySet input);
    void setOutput(FuzzySet output);
    FuzzySet getOutput(std::vector<float> positions);

private:
    std::vector<FuzzySet> p_inputs;
    FuzzySet p_output;

    float computeAlpha(std::vector<float> positions);

};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYRULE_H
