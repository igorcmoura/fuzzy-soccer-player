#include <iostream>
#include <math.h>

#include "fuzzy/fuzzy.h"
#include "fuzzy/fuzzy_collection.h"
#include "fuzzy/fuzzy_system.h"
#include "../lib/environm/environm.h"

#define PI 3.14159265358979323846264338327950f

int main( int argc, char* argv[] ) {

    ///conexão
    environm::soccer::clientEnvironm environment;

    if ( argc != 3 ) {
        printf( "\nSoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER\n" );
        return 0; // Cancela a execução se parâmetros inválidos.
    }

    // Conecta-se ao SoccerMatch. Supõe que SoccerMatch está rodando na máquina
    // local e que um dos robôs esteja na porta 1024. Porta e local podem mudar.
    if ( ! environment.connect(argv[1], atoi( argv[2]))) {
        printf( "\nFail connecting to the SoccerMatch.\n" );
        return 0; // Cancela a execução se não conseguiu conectar-se.
    }


    /// definição das regras e criação do sistema fuzzy
    fuzzy::Domain angle = fuzzy::Domain(-180, 180);

    // Cria o set do angulo da bola
    fuzzy::FuzzyCollection *ball_angle_c = new fuzzy::FuzzyCollection(angle);
    ball_angle_c->addSet(fuzzy::createDownLine(-180, 0, angle), "left");
    ball_angle_c->addSet(fuzzy::createTriangle(-90, 0, 90, angle), "front");
    ball_angle_c->addSet(fuzzy::createUpLine(0, 180, angle), "right");

    // Cria o set do angulo do alvo
    fuzzy::FuzzyCollection *goal_angle_c = new fuzzy::FuzzyCollection(angle);
    goal_angle_c->addSet(fuzzy::createDownLine(-180, 0, angle), "left");
    goal_angle_c->addSet(fuzzy::createTriangle(-90, 0, 90, angle), "front");
    goal_angle_c->addSet(fuzzy::createUpLine(0, 180, angle), "right");

    // Cria o set do angulo do robo
    fuzzy::FuzzyCollection *robot_angle_c = new fuzzy::FuzzyCollection(angle);
    robot_angle_c->addSet(fuzzy::createDownLine(-180, 0, angle), "left");
    robot_angle_c->addSet(fuzzy::createTriangle(-180, 0, 180, angle), "front");
    robot_angle_c->addSet(fuzzy::createUpLine(0, 180, angle), "right");

    // Cria regras
    fuzzy::FuzzySystem angle_system = fuzzy::FuzzySystem(angle);

    // Bola E, Alvo E -> Robot F
    fuzzy::FuzzyRule angle_rule1 = fuzzy::FuzzyRule();
    angle_rule1.addInput(ball_angle_c, "left");
    angle_rule1.addInput(goal_angle_c, "left");
    angle_rule1.setOutput(robot_angle_c, "front");
    angle_system.addRule(angle_rule1);

    // Bola F, Alvo E -> Robo D
    fuzzy::FuzzyRule angle_rule2 = fuzzy::FuzzyRule();
    angle_rule2.addInput(ball_angle_c, "front");
    angle_rule2.addInput(goal_angle_c, "left");
    angle_rule2.setOutput(robot_angle_c, "right");
    angle_system.addRule(angle_rule2);

    // Bola D, Alvo E -> Robo D
    fuzzy::FuzzyRule angle_rule3 = fuzzy::FuzzyRule();
    angle_rule3.addInput(ball_angle_c, "right");
    angle_rule3.addInput(goal_angle_c, "left");
    angle_rule3.setOutput(robot_angle_c, "right");
    angle_system.addRule(angle_rule3);

    // Bola E, Alvo F -> Robo E
    fuzzy::FuzzyRule angle_rule4 = fuzzy::FuzzyRule();
    angle_rule4.addInput(ball_angle_c, "left");
    angle_rule4.addInput(goal_angle_c, "front");
    angle_rule4.setOutput(goal_angle_c, "left");
    angle_system.addRule(angle_rule4);

    // Bola F Alvo F -> Robo F
    fuzzy::FuzzyRule angle_rule5 = fuzzy::FuzzyRule();
    angle_rule5.addInput(ball_angle_c, "front");
    angle_rule5.addInput(goal_angle_c, "front");
    angle_rule5.setOutput(robot_angle_c, "front");
    angle_system.addRule(angle_rule5);

    // Bola D, Alvo F -> Robo D
    fuzzy::FuzzyRule angle_rule6 = fuzzy::FuzzyRule();
    angle_rule6.addInput(ball_angle_c, "right");
    angle_rule6.addInput(goal_angle_c, "front");
    angle_rule6.setOutput(robot_angle_c, "right");
    angle_system.addRule(angle_rule6);

    // Bola E, Alvo D -> Robo E
    fuzzy::FuzzyRule angle_rule7 = fuzzy::FuzzyRule();
    angle_rule7.addInput(ball_angle_c, "left");
    angle_rule7.addInput(goal_angle_c, "right");
    angle_rule7.setOutput(goal_angle_c, "left");
    angle_system.addRule(angle_rule7);

    // Bola F, Alvo D -> Robo E
    fuzzy::FuzzyRule angle_rule8 = fuzzy::FuzzyRule();
    angle_rule8.addInput(ball_angle_c, "front");
    angle_rule8.addInput(goal_angle_c, "right");
    angle_rule8.setOutput(goal_angle_c, "left");
    angle_system.addRule(angle_rule8);

    // Bola D, Alvo D -> Robo F
    fuzzy::FuzzyRule angle_rule9 = fuzzy::FuzzyRule();
    angle_rule9.addInput(ball_angle_c, "right");
    angle_rule9.addInput(goal_angle_c, "right");
    angle_rule9.setOutput(robot_angle_c, "front");
    angle_system.addRule(angle_rule9);

    fuzzy::Domain speed_domain = fuzzy::Domain(-1, 1);
    fuzzy::FuzzySystem velocity_system = fuzzy::FuzzySystem(speed_domain);

    ///execução
    bool cont = true;
    float robot_angle, leftMotor, rightMotor, cosine, sine, velocity, ball_angle, goal_angle;
    while(cont) {

        ball_angle = environment.getBallAngle() * 180/PI;
        ball_angle_c->setInput(ball_angle);
        printf("Ball: %f\n", ball_angle);
        goal_angle = environment.getTargetAngle(environment.getOwnGoal()) * 180/PI;
        goal_angle_c->setInput(goal_angle);
        printf("Goal: %f\n", goal_angle);

        robot_angle = angle_system.getOutput();
        velocity = velocity_system.getOutput();

        cosine = (float)cos(robot_angle*PI/180);
        sine = (float)sin(robot_angle*PI/180);

        leftMotor = (velocity + cosine - sine)/10;
        rightMotor = (velocity + cosine + sine)/10;

        printf("Left: %f\tRight: %f -> Angle: %f\n", leftMotor, rightMotor, robot_angle);

        cont = environment.act(leftMotor, rightMotor);
    }
    return 0;
}
