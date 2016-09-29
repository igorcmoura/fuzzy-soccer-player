#include "fuzzy_player.h"

#include "fuzzy.h"

#define PI 3.14159265358979323846264338327950f

namespace fuzzy {

FuzzyPlayer::FuzzyPlayer(environm::soccer::clientEnvironm *environment)
    : environment_(environment) {
    float height = this->environment_->getWorldHeight(), width = this->environment_->getWorldWidth(), max_dist = (float) sqrt(pow(height, 2) + pow(width, 2));
    /// definição das regras e criação do sistema fuzzy
    Domain angle = Domain(-180, 180);
    Domain distance = Domain(0, max_dist);

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

    // Cria o set da distancia da bola
    ball_distance_ = new FuzzyCollection(distance);
    ball_distance_->addSet(createDownLine(0, max_dist/2, angle), "near");
    ball_distance_->addSet(createTriangle(max_dist/4, max_dist/2, 3*max_dist/4, angle), "medium");
    ball_distance_->addSet(createUpLine(max_dist/2, max_dist, angle), "far");

    // Cria regras
    system_ = new FuzzySystem(angle);

///bola longe
    // Bola E, Alvo E -> Robot E
    FuzzyRule angle_rule1 = FuzzyRule();
    angle_rule1.addInput(ball_distance_, "far");
    angle_rule1.addInput(ball_angle_, "left");
    angle_rule1.addInput(goal_angle_, "left");
    angle_rule1.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule1);
    // Bola F, Alvo E -> Robo F
    FuzzyRule angle_rule2 = FuzzyRule();
    angle_rule2.addInput(ball_distance_, "far");
    angle_rule2.addInput(ball_angle_, "front");
    angle_rule2.addInput(goal_angle_, "left");
    angle_rule2.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule2);
    // Bola D, Alvo E -> Robo D
    FuzzyRule angle_rule3 = FuzzyRule();
    angle_rule3.addInput(ball_distance_, "far");
    angle_rule3.addInput(ball_angle_, "right");
    angle_rule3.addInput(goal_angle_, "left");
    angle_rule3.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule3);
    // Bola E, Alvo F -> Robo E
    FuzzyRule angle_rule4 = FuzzyRule();
    angle_rule4.addInput(ball_distance_, "far");
    angle_rule4.addInput(ball_angle_, "left");
    angle_rule4.addInput(goal_angle_, "front");
    angle_rule4.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule4);
    // Bola F Alvo F -> Robo F
    FuzzyRule angle_rule5 = FuzzyRule();
    angle_rule5.addInput(ball_distance_, "far");
    angle_rule5.addInput(ball_angle_, "front");
    angle_rule5.addInput(goal_angle_, "front");
    angle_rule5.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule5);
    // Bola D, Alvo F -> Robo D
    FuzzyRule angle_rule6 = FuzzyRule();
    angle_rule6.addInput(ball_distance_, "far");
    angle_rule6.addInput(ball_angle_, "right");
    angle_rule6.addInput(goal_angle_, "front");
    angle_rule6.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule6);
    // Bola E, Alvo D -> Robo E
    FuzzyRule angle_rule7 = FuzzyRule();
    angle_rule7.addInput(ball_distance_, "far");
    angle_rule7.addInput(ball_angle_, "left");
    angle_rule7.addInput(goal_angle_, "right");
    angle_rule7.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule7);
    // Bola F, Alvo D -> Robo F
    FuzzyRule angle_rule8 = FuzzyRule();
    angle_rule8.addInput(ball_distance_, "far");
    angle_rule8.addInput(ball_angle_, "front");
    angle_rule8.addInput(goal_angle_, "right");
    angle_rule8.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule8);
    // Bola D, Alvo D -> Robo D
    FuzzyRule angle_rule9 = FuzzyRule();
    angle_rule9.addInput(ball_distance_, "far");
    angle_rule9.addInput(ball_angle_, "right");
    angle_rule9.addInput(goal_angle_, "right");
    angle_rule9.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule9);

///bola distancia média
    // Bola E, Alvo E -> Robot F
    FuzzyRule angle_rule10 = FuzzyRule();
    angle_rule10.addInput(ball_distance_, "medium");
    angle_rule10.addInput(ball_angle_, "left");
    angle_rule10.addInput(goal_angle_, "left");
    angle_rule10.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule10);
    // Bola F, Alvo E -> Robo D
    FuzzyRule angle_rule11 = FuzzyRule();
    angle_rule11.addInput(ball_distance_, "medium");
    angle_rule11.addInput(ball_angle_, "front");
    angle_rule11.addInput(goal_angle_, "left");
    angle_rule11.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule11);
    // Bola D, Alvo E -> Robo D
    FuzzyRule angle_rule12 = FuzzyRule();
    angle_rule12.addInput(ball_distance_, "medium");
    angle_rule12.addInput(ball_angle_, "right");
    angle_rule12.addInput(goal_angle_, "left");
    angle_rule12.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule12);
    // Bola E, Alvo F -> Robo E
    FuzzyRule angle_rule13 = FuzzyRule();
    angle_rule13.addInput(ball_distance_, "medium");
    angle_rule13.addInput(ball_angle_, "left");
    angle_rule13.addInput(goal_angle_, "front");
    angle_rule13.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule13);
    // Bola F Alvo F -> Robo F
    FuzzyRule angle_rule14 = FuzzyRule();
    angle_rule14.addInput(ball_distance_, "medium");
    angle_rule14.addInput(ball_angle_, "front");
    angle_rule14.addInput(goal_angle_, "front");
    angle_rule14.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule14);
    // Bola D, Alvo F -> Robo D
    FuzzyRule angle_rule15 = FuzzyRule();
    angle_rule15.addInput(ball_distance_, "medium");
    angle_rule15.addInput(ball_angle_, "right");
    angle_rule15.addInput(goal_angle_, "front");
    angle_rule15.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule15);
    // Bola E, Alvo D -> Robo E
    FuzzyRule angle_rule16 = FuzzyRule();
    angle_rule16.addInput(ball_distance_, "medium");
    angle_rule16.addInput(ball_angle_, "left");
    angle_rule16.addInput(goal_angle_, "right");
    angle_rule16.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule16);
    // Bola F, Alvo D -> Robo E
    FuzzyRule angle_rule17 = FuzzyRule();
    angle_rule17.addInput(ball_distance_, "medium");
    angle_rule17.addInput(ball_angle_, "front");
    angle_rule17.addInput(goal_angle_, "right");
    angle_rule17.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule17);
    // Bola D, Alvo D -> Robo F
    FuzzyRule angle_rule18 = FuzzyRule();
    angle_rule18.addInput(ball_distance_, "medium");
    angle_rule18.addInput(ball_angle_, "right");
    angle_rule18.addInput(goal_angle_, "right");
    angle_rule18.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule18);

    ///bola perto
    // Bola E, Alvo E -> Robot F
    FuzzyRule angle_rule19 = FuzzyRule();
    angle_rule19.addInput(ball_distance_, "medium");
    angle_rule19.addInput(ball_angle_, "left");
    angle_rule19.addInput(goal_angle_, "left");
    angle_rule19.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule19);
    // Bola F, Alvo E -> Robo D
    FuzzyRule angle_rule20 = FuzzyRule();
    angle_rule20.addInput(ball_distance_, "medium");
    angle_rule20.addInput(ball_angle_, "front");
    angle_rule20.addInput(goal_angle_, "left");
    angle_rule20.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule20);
    // Bola D, Alvo E -> Robo D
    FuzzyRule angle_rule21 = FuzzyRule();
    angle_rule21.addInput(ball_distance_, "medium");
    angle_rule21.addInput(ball_angle_, "right");
    angle_rule21.addInput(goal_angle_, "left");
    angle_rule21.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule21);
    // Bola E, Alvo F -> Robo E
    FuzzyRule angle_rule22 = FuzzyRule();
    angle_rule22.addInput(ball_distance_, "medium");
    angle_rule22.addInput(ball_angle_, "left");
    angle_rule22.addInput(goal_angle_, "front");
    angle_rule22.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule22);
    // Bola F Alvo F -> Robo F
    FuzzyRule angle_rule23 = FuzzyRule();
    angle_rule23.addInput(ball_distance_, "medium");
    angle_rule23.addInput(ball_angle_, "front");
    angle_rule23.addInput(goal_angle_, "front");
    angle_rule23.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule23);
    // Bola D, Alvo F -> Robo D
    FuzzyRule angle_rule24 = FuzzyRule();
    angle_rule24.addInput(ball_distance_, "medium");
    angle_rule24.addInput(ball_angle_, "right");
    angle_rule24.addInput(goal_angle_, "front");
    angle_rule24.setOutput(robot_angle_, "right");
    system_->addRule(angle_rule24);
    // Bola E, Alvo D -> Robo E
    FuzzyRule angle_rule25 = FuzzyRule();
    angle_rule25.addInput(ball_distance_, "medium");
    angle_rule25.addInput(ball_angle_, "left");
    angle_rule25.addInput(goal_angle_, "right");
    angle_rule25.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule25);
    // Bola F, Alvo D -> Robo E
    FuzzyRule angle_rule26 = FuzzyRule();
    angle_rule26.addInput(ball_distance_, "medium");
    angle_rule26.addInput(ball_angle_, "front");
    angle_rule26.addInput(goal_angle_, "right");
    angle_rule26.setOutput(robot_angle_, "left");
    system_->addRule(angle_rule26);
    // Bola D, Alvo D -> Robo F
    FuzzyRule angle_rule27 = FuzzyRule();
    angle_rule27.addInput(ball_distance_, "medium");
    angle_rule27.addInput(ball_angle_, "right");
    angle_rule27.addInput(goal_angle_, "right");
    angle_rule27.setOutput(robot_angle_, "front");
    system_->addRule(angle_rule27);
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
    float robot_angle = system_->getOutput();
    float cosine = (float)cos(robot_angle*PI/180);
    float sine = (float)sin(robot_angle*PI/180);

    left_motor_ = ( cosine - sine)/2;
    right_motor_ = ( cosine + sine)/2;
    printf("Left: %f\tRight: %f -> Angle: %f\n", left_motor_, right_motor_, robot_angle);
}

bool FuzzyPlayer::actuate() {
    return environment_->act(left_motor_, right_motor_);
}
} // namespace fuzzy