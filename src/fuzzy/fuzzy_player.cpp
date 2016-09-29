#include "fuzzy_player.h"

#include "fuzzy.h"

#define PI 3.14159265358979323846264338327950f

namespace fuzzy {

FuzzyPlayer::FuzzyPlayer(environm::soccer::clientEnvironm *environment)
    : environment_(environment) {

    /// definição das regras e criação do sistema fuzzy
    Domain angle = Domain(-180, 180);

    // Cria o set do angulo da bola
    ball_angle_ = new FuzzyCollection(angle);
    ball_angle_->addSet(createDownLine(-180, 0, angle), "left");
    ball_angle_->addSet(createTriangle(-90, 0, 90, angle), "front");
    ball_angle_->addSet(createUpLine(0, 180, angle), "right");

    // Cria o set do angulo do alvo
    goal_angle_ = new FuzzyCollection(angle);
    goal_angle_->addSet(createDownLine(-180, 0, angle), "left");
    goal_angle_->addSet(createTriangle(-90, 0, 90, angle), "front");
    goal_angle_->addSet(createUpLine(0, 180, angle), "right");

    // Cria o set do angulo do robo
    robot_angle_ = new FuzzyCollection(angle);
    robot_angle_->addSet(createDownLine(-180, 0, angle), "left");
    robot_angle_->addSet(createTriangle(-180, 0, 180, angle), "front");
    robot_angle_->addSet(createUpLine(0, 180, angle), "right");

    // Cria regras
    angle_system_ = new FuzzySystem(angle);

    // Bola E, Alvo E -> Robot F
    FuzzyRule angle_rule1 = FuzzyRule();
    angle_rule1.addInput(ball_angle_, "left");
    angle_rule1.addInput(goal_angle_, "left");
    angle_rule1.setOutput(robot_angle_, "front");
    angle_system_->addRule(angle_rule1);

    // Bola F, Alvo E -> Robo D
    FuzzyRule angle_rule2 = FuzzyRule();
    angle_rule2.addInput(ball_angle_, "front");
    angle_rule2.addInput(goal_angle_, "left");
    angle_rule2.setOutput(robot_angle_, "right");
    angle_system_->addRule(angle_rule2);

    // Bola D, Alvo E -> Robo D
    FuzzyRule angle_rule3 = FuzzyRule();
    angle_rule3.addInput(ball_angle_, "right");
    angle_rule3.addInput(goal_angle_, "left");
    angle_rule3.setOutput(robot_angle_, "right");
    angle_system_->addRule(angle_rule3);

    // Bola E, Alvo F -> Robo E
    FuzzyRule angle_rule4 = FuzzyRule();
    angle_rule4.addInput(ball_angle_, "left");
    angle_rule4.addInput(goal_angle_, "front");
    angle_rule4.setOutput(robot_angle_, "left");
    angle_system_->addRule(angle_rule4);

    // Bola F Alvo F -> Robo F
    FuzzyRule angle_rule5 = FuzzyRule();
    angle_rule5.addInput(ball_angle_, "front");
    angle_rule5.addInput(goal_angle_, "front");
    angle_rule5.setOutput(robot_angle_, "front");
    angle_system_->addRule(angle_rule5);

    // Bola D, Alvo F -> Robo D
    FuzzyRule angle_rule6 = FuzzyRule();
    angle_rule6.addInput(ball_angle_, "right");
    angle_rule6.addInput(goal_angle_, "front");
    angle_rule6.setOutput(robot_angle_, "right");
    angle_system_->addRule(angle_rule6);

    // Bola E, Alvo D -> Robo E
    FuzzyRule angle_rule7 = FuzzyRule();
    angle_rule7.addInput(ball_angle_, "left");
    angle_rule7.addInput(goal_angle_, "right");
    angle_rule7.setOutput(robot_angle_, "left");
    angle_system_->addRule(angle_rule7);

    // Bola F, Alvo D -> Robo E
    FuzzyRule angle_rule8 = FuzzyRule();
    angle_rule8.addInput(ball_angle_, "front");
    angle_rule8.addInput(goal_angle_, "right");
    angle_rule8.setOutput(robot_angle_, "left");
    angle_system_->addRule(angle_rule8);

    // Bola D, Alvo D -> Robo F
    FuzzyRule angle_rule9 = FuzzyRule();
    angle_rule9.addInput(ball_angle_, "right");
    angle_rule9.addInput(goal_angle_, "right");
    angle_rule9.setOutput(robot_angle_, "front");
    angle_system_->addRule(angle_rule9);

    Domain speed_domain = Domain(-1, 1);
    speed_system_ = new FuzzySystem(speed_domain);
}

void FuzzyPlayer::readInputs() {
    float ball_angle = environment_->getBallAngle() * 180/PI;
    ball_angle_->setInput(ball_angle);
    printf("Ball: %f\n", ball_angle);

    float goal_angle = environment_->getTargetAngle(environment_->getOwnGoal()) * 180/PI;
    goal_angle_->setInput(goal_angle);
    printf("Goal: %f\n", goal_angle);
}

void FuzzyPlayer::process() {
    float robot_angle = angle_system_->getOutput();
    float cosine = (float)cos(robot_angle*PI/180);
    float sine = (float)sin(robot_angle*PI/180);
    float speed = speed_system_->getOutput();

    left_motor_ = (speed + cosine - sine)/10;
    right_motor_ = (speed + cosine + sine)/10;
    printf("Left: %f\tRight: %f -> Angle: %f\n", left_motor_, right_motor_, robot_angle);
}

bool FuzzyPlayer::actuate() {
    return environment_->act(left_motor_, right_motor_);
}

} // namespace fuzzy
