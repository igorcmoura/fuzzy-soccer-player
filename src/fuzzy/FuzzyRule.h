//
// Created by elRubo on 13/09/2016.
//

#ifndef FUZZYSOCCERPLAYER_FUZZYRULE_H
#include<vector>
#include<algorithm> //for std::min
#define FUZZYSOCCERPLAYER_FUZZYRULE_H


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

#endif //FUZZYSOCCERPLAYER_FUZZYRULE_H
