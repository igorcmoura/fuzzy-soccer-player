#include <iostream>
#include "fuzzy/fuzzy.h"
#include "fuzzy/fuzzy_system.h"
#include <math.h>
#include "../lib/environm/environm.h"

int main( int argc, char* argv[] ) {


    /// definição das regras e criação do sistema fuzzy
    fuzzy::Domain angle = fuzzy::Domain(-180, 180);

    // Cria o set do angulo da bola
    fuzzy::FuzzySet ball_left = fuzzy::createDownLine(-180, 0, angle);
    fuzzy::FuzzySet ball_front = fuzzy::createTriangle(-90, 0, 90, angle);
    fuzzy::FuzzySet ball_right = fuzzy::createUpLine(0, 180, angle);

    // Cria o set do angulo do alvo
    fuzzy::FuzzySet goal_left = fuzzy::createDownLine(-180, 0, angle);
    fuzzy::FuzzySet goal_front = fuzzy::createTriangle(-90, 0, 90, angle);
    fuzzy::FuzzySet goal_right = fuzzy::createUpLine(0, 180, angle);

    // Cria o set do angulo do robo
    fuzzy::FuzzySet robot_left = fuzzy::createDownLine(-180, 0, angle);
    fuzzy::FuzzySet robot_front = fuzzy::createTriangle(-180, 0, 180, angle);
    fuzzy::FuzzySet robot_right = fuzzy::createUpLine(0, 180, angle);

    // Cria regras
    fuzzy::FuzzySystem system = fuzzy::FuzzySystem();

    // Bola E, Alvo E -> Robot F
    fuzzy::FuzzyRule rule1 = fuzzy::FuzzyRule();
    rule1.addInput(ball_left);
    rule1.addInput(goal_left);
    rule1.setOutput(robot_front);
    system.addRule(rule1);

    // Bola F, Alvo E -> Robo D
    fuzzy::FuzzyRule rule2 = fuzzy::FuzzyRule();
    rule2.addInput(ball_front);
    rule2.addInput(goal_left);
    rule2.setOutput(robot_right);
    system.addRule(rule2);

    // Bola D, Alvo E -> Robo D
    fuzzy::FuzzyRule rule3 = fuzzy::FuzzyRule();
    rule3.addInput(ball_right);
    rule3.addInput(goal_left);
    rule3.setOutput(robot_right);
    system.addRule(rule3);

    // Bola E, Alvo F -> Robo E
    fuzzy::FuzzyRule rule4 = fuzzy::FuzzyRule();
    rule4.addInput(ball_left);
    rule4.addInput(goal_front);
    rule4.setOutput(robot_left);
    system.addRule(rule4);

    // Bola F Alvo F -> Robo F
    fuzzy::FuzzyRule rule5 = fuzzy::FuzzyRule();
    rule5.addInput(ball_front);
    rule5.addInput(goal_front);
    rule5.setOutput(robot_front);
    system.addRule(rule5);

    // Bola D, Alvo F -> Robo D
    fuzzy::FuzzyRule rule6 = fuzzy::FuzzyRule();
    rule6.addInput(ball_right);
    rule6.addInput(goal_front);
    rule6.setOutput(robot_right);
    system.addRule(rule6);

    // Bola E, Alvo D -> Robo E
    fuzzy::FuzzyRule rule7 = fuzzy::FuzzyRule();
    rule7.addInput(ball_left);
    rule7.addInput(goal_right);
    rule7.setOutput(robot_left);
    system.addRule(rule7);

    // Bola F, Alvo D -> Robo E
    fuzzy::FuzzyRule rule8 = fuzzy::FuzzyRule();
    rule8.addInput(ball_front);
    rule8.addInput(goal_right);
    rule8.setOutput(robot_left);
    system.addRule(rule8);

    // Bola D, Alvo D -> Robo F
    fuzzy::FuzzyRule rule9 = fuzzy::FuzzyRule();
    rule9.addInput(ball_right);
    rule9.addInput(goal_right);
    rule9.setOutput(robot_front);
    system.addRule(rule9);
    printf("miau\n");

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
    printf("olar 1\n");

    ///execução
    bool cont = true;
    std::vector<float> positions;       //ordem das entradas: 0 -> ball angle / 1 -> target angle
    printf("oba oba oba\n");
    float robot_angle, leftMotor, rightMotor;
    printf("nasusnausaunsa \n");
    while(cont) {
        printf("olar");
        positions.clear();
        positions.push_back(environment.getBallAngle());
        positions.push_back(environment.getTargetAngle( environment.getOwnGoal()));
        robot_angle = system.getOutput(positions);  //fuzzy magic happens here
        leftMotor = (float) (cos(robot_angle) + sin(robot_angle));
        rightMotor =(float) (cos(robot_angle) - sin(robot_angle));
        cont = environment.act(leftMotor, rightMotor);  // Envia o dado pra plataforma. Termina a execução se falha ao agir.
    }
    return 0;
}