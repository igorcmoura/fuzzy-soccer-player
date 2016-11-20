#include <iostream>
#include <math.h>

#include "fuzzy/fuzzy_player.h"
#include "logging/environment_logger.h"
#include "../lib/environm/environm.h"

int main( int argc, char* argv[] ) {

    int debug_mode = 0;
    const char *log_file_name;
    environm::soccer::clientEnvironm *environment = new environm::soccer::clientEnvironm();

    if ( argc < 3 ) {
        std::cout << "\nSoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER" << std::endl;
        return 0; // Invalid parameters, cancel execution
    }

    // Connect to the SoccerMatch
    if ( ! environment->connect(argv[1], atoi( argv[2]))) {
        std::cout << "\nFail connecting to the SoccerMatch." << std::endl;
        return 0; // Couldn't connect, cancel execution
    }

    // Logging file
    if (argc >= 4) {
        log_file_name = argv[3];
    } else {
        log_file_name = "fuzzy.log";
    }
    std::cout << "Logging to file " << log_file_name << std::endl;

    // Check debug mode
    if (argc >= 5 and argv[4] != "0") {
        std::cout << "Debug mode enabled" << std::endl;
        debug_mode = atoi(argv[4]);
    }

    fuzzy::FuzzyPlayer player = fuzzy::FuzzyPlayer(environment, debug_mode);
    logging::EnvironmentLogger logger = logging::EnvironmentLogger(environment, log_file_name, 5000, true);

    // Execution
    bool continue_execution = true;
    double player_output[2];
    while(continue_execution) {
        player.readInputs();
        player.process(player_output);
        continue_execution = player.act();

        if (debug_mode > 0)
            getchar();
        if (debug_mode > 1) {
            std::cout << "getOwnScore: " << environment->getOwnScore() << std::endl;
            std::cout << "getRivalScore: " << environment->getRivalScore() << std::endl;
            std::cout << "getWorldWidth: " << environment->getWorldWidth()/2 << std::endl;
            std::cout << "getWorldHeight: " << environment->getWorldHeight()/2 << std::endl;
            std::cout << "getGoalDeep: " << environment->getGoalDeep() << std::endl;
            std::cout << "getGoalLength: " << environment->getGoalLength() << std::endl;
            std::cout << "getRobotRadius: " << environment->getRobotRadius() << std::endl;
            std::cout << "getDistance: " << environment->getDistance() << std::endl;
            std::cout << "getBallAngle: " << environment->getBallAngle() << std::endl;
            std::cout << "getGoalAngle: " << environment->getTargetAngle(environment->getOwnGoal()) << std::endl;
            std::cout << "getRivalGoalAngle: " << environment->getTargetAngle(environment->getRivalGoal()) << std::endl;
            std::cout << "getCollision: " << environment->getCollision() << std::endl;
            std::cout << "getObstacleAngle: " << environment->getObstacleAngle() << std::endl;
            std::cout << "getSpin: " << environment->getSpin() << std::endl;
        }
        std::cout << "========================================" << std::endl;

        logger.updateLog(player_output);
    }
    logger.stop();
    return 0;
}
