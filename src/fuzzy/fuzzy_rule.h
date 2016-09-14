#ifndef FUZZYSOCCERPLAYER_FUZZYRULE_H
#define FUZZYSOCCERPLAYER_FUZZYRULE_H

#include<vector>
#include<algorithm> //for std::min

#include "fuzzy_set.h"

namespace fuzzy {

class FuzzyRule {
public:
    FuzzyRule();
    void AddInput(FuzzySet input, float position);
    void setOutput(FuzzySet output);
    FuzzySet getOutput();

private:
    std::vector<FuzzySet> p_inputs;
    FuzzySet p_output;
    float p_alpha;
    std::vector<float> p_positions;
    void p_computeAlpha();

};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYRULE_H
