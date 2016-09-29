#include "fuzzy_player.h"

#include "fuzzy.h"

#define PI 3.14159265358979323846264338327950f

namespace fuzzy {

FuzzyPlayer::FuzzyPlayer(environm::soccer::clientEnvironm *environment)
    : environment_(environment) {

    float height = this->environment_->getWorldHeight(), width = this->environment_->getWorldWidth(), max_dist = (float) sqrt(pow(height, 2) + pow(width, 2));
    // Rules definition

    Domain angle = Domain(-180, 180);
    Domain distance = Domain(0, max_dist);

    // Ball angle sets
    ball_angle_ = new FuzzyCollection(angle);
    ball_angle_->addSet(createDownLine(-180, 0, angle), "left");
    ball_angle_->addSet(createTriangle(-90, 0, 90, angle), "front");
    ball_angle_->addSet(createUpLine(0, 180, angle), "right");

    // Ball distance sets
    ball_distance_ = new FuzzyCollection(distance);
    ball_distance_->addSet(createDownLine(0, max_dist/2, angle), "near");
    ball_distance_->addSet(createTriangle(max_dist/4, max_dist/2, 3*max_dist/4, angle), "medium");
    ball_distance_->addSet(createUpLine(max_dist/2, max_dist, angle), "far");

    // Goal angle sets
    goal_angle_ = new FuzzyCollection(angle);
    goal_angle_->addSet(createDownLine(-180, 0, angle), "left");
    goal_angle_->addSet(createTriangle(-90, 0, 90, angle), "front");
    goal_angle_->addSet(createUpLine(0, 180, angle), "right");

    // Robot angle sets
    robot_angle_ = new FuzzyCollection(angle);
    robot_angle_->addSet(createDownLine(-180, 0, angle), "left");
    robot_angle_->addSet(createTriangle(-180, 0, 180, angle), "front");
    robot_angle_->addSet(createUpLine(0, 180, angle), "right");

    // Define rules
    system_ = new FuzzySystem(angle);

    // Ball distance far
    {   // Ball L, Goal L -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball F, Goal L -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
    {   // Ball R, Goal L -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball L, Goal F -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball F Goal F -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
    {   // Ball R, Goal F -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball L, Goal R -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball F, Goal R -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
    {   // Ball R, Goal R -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }

    // Ball distance medium
    {   // Ball L, Goal L -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
    {   // Ball F, Goal L -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball R, Goal L -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball L, Goal F -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball F Goal F -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
    {   // Ball R, Goal F -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball L, Goal R -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball F, Goal R -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball R, Goal R -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }

    // Ball distance near
    {   // Ball L, Goal L -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
    {   // Ball F, Goal L -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball R, Goal L -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball L, Goal F -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball F Goal F -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
    {   // Ball R, Goal F -> Robot R
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "right");
        system_->addRule(rule);
    }
    {   // Ball L, Goal R -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball F, Goal R -> Robot L
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        system_->addRule(rule);
    }
    {   // Ball R, Goal R -> Robot F
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "front");
        system_->addRule(rule);
    }
}

void FuzzyPlayer::readInputs() {
    float ball_angle = environment_->getBallAngle() * 180/PI;
    ball_angle_->setInput(ball_angle);
    printf("Ball angle: %f\n", ball_angle);

    float goal_angle = environment_->getTargetAngle(environment_->getOwnGoal()) * 180/PI;
    goal_angle_->setInput(goal_angle);
    printf("Goal angle: %f\n", goal_angle);
}

void FuzzyPlayer::process() {
    float robot_angle = system_->getOutput();
    float cosine = (float)cos(robot_angle*PI/180);
    float sine = (float)sin(robot_angle*PI/180);

    left_motor_ = (cosine - sine)/2;
    right_motor_ = (cosine + sine)/2;
    printf("Left: %f\tRight: %f -> Angle: %f\n", left_motor_, right_motor_, robot_angle);
}

bool FuzzyPlayer::act() {
    return environment_->act(left_motor_, right_motor_);
}

} // namespace fuzzy
