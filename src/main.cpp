#include <iostream>
#include <math.h>

#include "fuzzy/fuzzy_player.h"
#include "../lib/environm/environm.h"

int main( int argc, char* argv[] ) {

    environm::soccer::clientEnvironm *environment = new environm::soccer::clientEnvironm();

    if ( argc != 3 ) {
        printf( "\nSoccerPlayer SERVER_ADDRESS_STRING SERVER_PORT_NUMBER\n" );
        return 0; // Invalid parameters, cancel execution
    }

    // Connect to the SoccerMatch
    if ( ! environment->connect(argv[1], atoi( argv[2]))) {
        printf( "\nFail connecting to the SoccerMatch.\n" );
        return 0; // Couldn't connect, cancel execution
    }

    fuzzy::FuzzyPlayer player = fuzzy::FuzzyPlayer(environment);

    // Execution
    bool continue_execution = true;
    while(continue_execution) {
        player.readInputs();
        player.process();
        continue_execution = player.act();
    }
    return 0;
}
