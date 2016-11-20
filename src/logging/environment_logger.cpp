#include "environment_logger.h"

#define LINE_CHARS 70

namespace logging {

EnvironmentLogger::EnvironmentLogger(environm::soccer::clientEnvironm *environment, const char *output_file_name, int lines, bool only_victories)
        : environment_(environment), lines_limit_(lines), save_only_victories_(only_victories) {

    stack_ = (char*) malloc(LINE_CHARS * lines_limit_ * sizeof(char));
    cleanStack();

    output_file_.open(output_file_name);
}

void EnvironmentLogger::updateLog(double *player_output) {
    if (playerScored()) {
        saveLog();
    }
    if (rivalScored()) {
        if (save_only_victories_) {
            cleanStack();
        } else {
            saveLog();
        }
    }

    double distance = environment_->getDistance();
    double ball_angle = environment_->getBallAngle();
    double target_angle = environment_->getTargetAngle(environment_->getOwnGoal());
    double collision = environment_->getCollision();
    double obstacle_angle = environment_->getObstacleAngle();
    double spin = environment_->getSpin();

    sprintf(&(stack_[stack_ptr_ * LINE_CHARS]), " %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
            distance,
            ball_angle,
            target_angle,
            collision,
            obstacle_angle,
            spin,
            player_output[0],
            player_output[1]);

    stack_ptr_++;
    if (stack_ptr_ >= lines_limit_)
        stack_ptr_ = 0;
    if (data_in_stack_ < lines_limit_)
        data_in_stack_++;
}

void EnvironmentLogger::saveLog() {
    for (; data_in_stack_ > 0; data_in_stack_--) {
        stack_ptr_--;
        if (stack_ptr_ < 0)
            stack_ptr_ = lines_limit_-1;
        output_file_ << &(stack_[stack_ptr_ * LINE_CHARS]);
    }
    output_file_.flush();
    cleanStack();
}

void EnvironmentLogger::cleanStack() {
    stack_ptr_ = 0;
    data_in_stack_ = 0;
}

bool EnvironmentLogger::playerScored() {
    static int last_score = 0;
    int current_score = environment_->getOwnScore();

    if (current_score > last_score) {
        last_score = current_score;
        return true;
    } else
        return false;
}

bool EnvironmentLogger::rivalScored() {
    static int last_score = 0;
    int current_score = environment_->getRivalScore();

    if (current_score > last_score) {
        last_score = current_score;
        return true;
    } else
        return false;
}

void EnvironmentLogger::stop() {
    output_file_.close();
}

} // namespace logging
