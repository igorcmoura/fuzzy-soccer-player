#ifndef FUZZYSOCCERPLAYER_FUZZY_PLAYER_H
#define FUZZYSOCCERPLAYER_FUZZY_PLAYER_H

#include <fstream>
#include <iostream>
#include <map>
#include <tuple>

#include "fuzzy_collection.h"
#include "fuzzy_system.h"
#include "../../lib/environm/environm.h"

namespace fuzzy {

class FuzzyPlayer {
public:
    FuzzyPlayer(environm::soccer::clientEnvironm *environment, int debug_mode);

    void readInputs();
    void process(double *output);
    bool act();

private:
    environm::soccer::clientEnvironm *environment_;
    int debug_mode_;

    FuzzySystem *angle_system_, *speed_system_;
    std::map<std::string, Domain*> domains_;
    std::map<std::string, FuzzyCollection*> collections_;
    std::vector<std::tuple<std::string, std::string>> inputs_definition_;

    float left_motor_, right_motor_;

    void readRulesFileElseDefault();

    void readRulesFile(std::ifstream *rules_file);
    void setRulesToDefault();

    void createDomainFromString(std::string domain_str);
    void createCollectionFromString(std::string collection_str);
    void createSetFromString(std::string set_str);
    void createRuleFromString(std::string rule_str);
    void createInputDefinitionFromString(std::string input_definition_str);

    float evaluateExpression(std::string str);
    float interpretValue(std::string str);
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZY_PLAYER_H
