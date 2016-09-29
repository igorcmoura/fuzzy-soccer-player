#include "fuzzy_player.h"

#include "fuzzy.h"

#define PI 3.14159265358979323846264338327950f
#define MAX_SPEED 1

namespace fuzzy {

FuzzyPlayer::FuzzyPlayer(environm::soccer::clientEnvironm *environment)
    : environment_(environment) {

    // Environment gives height, width and goal deep as the double it actually is
    float height = environment_->getWorldHeight()/2;
    // We have also to ignore the goals space in the width
    float width = (environment_->getWorldWidth()/2) - environment_->getGoalDeep();
    float max_dist = (float) sqrt(pow(height, 2) + pow(width, 2));

    float robot_radius = environment_->getRobotRadius();

    // Rules definition
    Domain angle = Domain(-180, 180);
    Domain distance = Domain(0, max_dist);
    Domain speed = Domain(-MAX_SPEED/3, MAX_SPEED);

    // Ball angle sets
    ball_angle_ = new FuzzyCollection(angle);
    ball_angle_->addSet(createDownLine(-180, -90, angle), "left_back");
    ball_angle_->addSet(createTriangle(-180, -90, 0, angle), "left");
    ball_angle_->addSet(createTriangle(-90, 0, 90, angle), "front");
    ball_angle_->addSet(createTriangle(0, 90, 180, angle), "right");
    ball_angle_->addSet(createUpLine(-90, 0, angle), "right_back");

    // Ball distance sets
    ball_distance_ = new FuzzyCollection(distance);
    ball_distance_->addSet(createDownLine(0, robot_radius, distance), "near");
    ball_distance_->addSet(createTrapezium(0, robot_radius*2, max_dist/2, max_dist, distance), "medium");
    ball_distance_->addSet(createUpLine(max_dist/2, max_dist, distance), "far");

    // Goal angle sets
    goal_angle_ = new FuzzyCollection(angle);
    goal_angle_->addSet(createDownLine(-180, -90, angle), "left_back");
    goal_angle_->addSet(createTriangle(-180, -90, 0, angle), "left");
    goal_angle_->addSet(createTriangle(-90, 0, 90, angle), "front");
    goal_angle_->addSet(createTriangle(0, 90, 180, angle), "right");
    goal_angle_->addSet(createUpLine(-90, 0, angle), "right_back");

    // Robot angle sets
    robot_angle_ = new FuzzyCollection(angle);
    robot_angle_->addSet(createDownLine(-180, -90, angle), "left_back");
    robot_angle_->addSet(createTriangle(-180, -90, 0, angle), "left");
    robot_angle_->addSet(createTriangle(-90, 0, 90, angle), "front");
    robot_angle_->addSet(createTriangle(0, 90, 180, angle), "right");
    robot_angle_->addSet(createUpLine(-90, 0, angle), "right_back");

    // Robot speed sets
    robot_speed_ = new FuzzyCollection(speed);
    robot_speed_->addSet(createDownLine(-0.01f, 0, speed), "rear");
    robot_speed_->addSet(createTriangle(0, 0, MAX_SPEED, speed), "slow");
    robot_speed_->addSet(createTriangle(0, MAX_SPEED/2, MAX_SPEED, speed), "medium");
    robot_speed_->addSet(createUpLine(0, MAX_SPEED, speed), "fast");

    // Define rules
    angle_system_ = new FuzzySystem(angle);
    speed_system_ = new FuzzySystem(speed);

    // Ball distance far
    {   // Ball left -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "left");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball left back -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "left_back");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball front -> Robot front
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "front");
        rule.setOutput(robot_angle_, "front");
        angle_system_->addRule(rule);
    }
    {   // Ball right -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "right");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball right back -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "right_back");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball front -> Robot fast
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "front");
        rule.setOutput(robot_speed_, "fast");
        speed_system_->addRule(rule);
    }
    {   // Ball left -> Robot speed medium
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "left");
        rule.setOutput(robot_speed_, "medium");
        speed_system_->addRule(rule);
    }
    {   // Ball right -> Robot speed medium
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "right");
        rule.setOutput(robot_speed_, "medium");
        speed_system_->addRule(rule);
    }
    {   // Ball left back -> Robot speed rear
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "left_back");
        rule.setOutput(robot_speed_, "slow");
        speed_system_->addRule(rule);
    }
    {   // Ball right back -> Robot speed rear
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "far");
        rule.addInput(ball_angle_, "right_back");
        rule.setOutput(robot_speed_, "rear");
        speed_system_->addRule(rule);
    }

    // Ball distance medium
    // Goal front
    {   // Ball front, Goal front -> Robot front
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "front");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal front -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal front -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal front -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal front -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    // Goal left
    {   // Ball front, Goal left -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball front, Goal left back -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal left -> Robot front
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "front");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal left back -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal left -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal left back -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal left -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal left back -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal left -> Robot left back TODO check this
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal left back -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    // Goal right
    {   // Ball front, Goal right -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball front, Goal right back -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal right -> Robot front
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "front");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal right back -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal right -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal right back -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal right -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal right back -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal right -> Robot right back TODO check this
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal right back -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball front -> Robot fast
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "front");
        rule.setOutput(robot_speed_, "fast");
        speed_system_->addRule(rule);
    }
    {   // Ball left -> Robot speed medium
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left");
        rule.setOutput(robot_speed_, "medium");
        speed_system_->addRule(rule);
    }
    {   // Ball right -> Robot speed medium
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right");
        rule.setOutput(robot_speed_, "medium");
        speed_system_->addRule(rule);
    }
    {   // Ball left back -> Robot speed medium
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "left_back");
        rule.setOutput(robot_speed_, "medium");
        speed_system_->addRule(rule);
    }
    {   // Ball right back -> Robot speed medium
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "medium");
        rule.addInput(ball_angle_, "right_back");
        rule.setOutput(robot_speed_, "medium");
        speed_system_->addRule(rule);
    }

    // Ball distance near
    // Goal front
    {   // Ball front Goal front -> Robot front
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "front");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal front -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal front -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal front -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal front -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "front");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    // Goal left
    {   // Ball front, Goal left -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal left -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal left -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal left back -> Robot front
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "front");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal left back -> Robot left back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal left -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal left -> Robot left back TODO check this
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "left");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal left back -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal left back -> Robot left back TODO check this
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "left_back");
        rule.setOutput(robot_angle_, "left_back");
        angle_system_->addRule(rule);
    }
    // Goal right
    {   // Ball front, Goal right -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "front");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal right -> Robot right
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "right");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal right -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball right, Goal right back -> Robot front
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "front");
        angle_system_->addRule(rule);
    }
    {   // Ball right back, Goal right back -> Robot right back
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "right_back");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal right -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal right -> Robot right back TODO check this
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "right");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Ball left, Goal right back -> Robot left
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "left");
        angle_system_->addRule(rule);
    }
    {   // Ball left back, Goal right back -> Robot right back TODO check this
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.addInput(ball_angle_, "left_back");
        rule.addInput(goal_angle_, "right_back");
        rule.setOutput(robot_angle_, "right_back");
        angle_system_->addRule(rule);
    }
    {   // Robot slow
        FuzzyRule rule = FuzzyRule();
        rule.addInput(ball_distance_, "near");
        rule.setOutput(robot_speed_, "slow");
        speed_system_->addRule(rule);
    }
}

void FuzzyPlayer::readInputs() {
    float ball_angle = environment_->getBallAngle() * 180/PI;
    ball_angle_->setInput(ball_angle);
    printf("Ball angle: %f\n", ball_angle);

    float ball_distance = environment_->getDistance();
    ball_distance_->setInput(ball_distance);
    printf("Ball distance: %f\n", ball_distance);

    float goal_angle = environment_->getTargetAngle(environment_->getOwnGoal()) * 180/PI;
    goal_angle_->setInput(goal_angle);
    printf("Goal angle: %f\n", goal_angle);
}

void FuzzyPlayer::process() {
    float robot_angle = angle_system_->getOutput();
    float cosine = (float)cos(robot_angle*PI/180);
    float sine = (float)sin(robot_angle*PI/180);

    float robot_speed = speed_system_->getOutput();
    printf("Robot angle -> %f\n", robot_angle);
    printf("Robot speed -> %f\n", robot_speed);

    left_motor_ = (robot_speed + cosine - sine)/6;
    right_motor_ = (robot_speed + cosine + sine)/6;
    printf("Left: %f\tRight: %f", left_motor_, right_motor_);
}

bool FuzzyPlayer::act() {
    return environment_->act(left_motor_, right_motor_);
}

} // namespace fuzzy
