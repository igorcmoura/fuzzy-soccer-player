#ifndef FUZZYSOCCERPLAYER_ENVIRONMENT_LOGGER_H
#define FUZZYSOCCERPLAYER_ENVIRONMENT_LOGGER_H

#include <iostream>
#include <fstream>

#include "../../lib/environm/environm.h"

namespace logging {

class EnvironmentLogger {
public:
    EnvironmentLogger(environm::soccer::clientEnvironm *environment, const char *output_file_name, int lines, bool only_victories = true);

    void updateLog(double *player_output);
    void stop();

private:
    environm::soccer::clientEnvironm *environment_;
    bool save_only_victories_;

    int stack_ptr_, data_in_stack_;
    char *stack_;
    int lines_limit_;

    std::ofstream output_file_;

    void cleanStack();

    bool playerScored();
    bool rivalScored();

    void saveLog();
};

} // namespace logging

#endif //FUZZYSOCCERPLAYER_ENVIRONMENT_LOGGER_H
