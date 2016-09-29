#ifndef FUZZYSOCCERPLAYER_FUZZY_PLAYER_H
#define FUZZYSOCCERPLAYER_FUZZY_PLAYER_H

#include "fuzzy_collection.h"
#include "fuzzy_system.h"
#include "../../lib/environm/environm.h"

namespace fuzzy {

class FuzzyPlayer {
public:
    FuzzyPlayer(environm::soccer::clientEnvironm *environment);

    void readInputs();
    void process();
    bool act();

private:
    environm::soccer::clientEnvironm *environment_;

    FuzzySystem *angle_system_, *speed_system_;
    FuzzyCollection *ball_angle_, *goal_angle_, *ball_distance_;
    FuzzyCollection *robot_angle_, *robot_speed_;

    float left_motor_, right_motor_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZY_PLAYER_H
