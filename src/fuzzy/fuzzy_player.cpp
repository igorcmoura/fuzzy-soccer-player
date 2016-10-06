#include "fuzzy_player.h"

#include <algorithm>

#include "fuzzy.h"

#define PI 3.14159265358979323846264338327950f
#define MAX_SPEED 1

// Rules file tokens
#define COLLECTION_TOKEN "c"
#define DOMAIN_TOKEN "d"
#define SET_TOKEN "s"
#define RULE_TOKEN "r"
#define INPUT_DEFINITION_TOKEN "i"

namespace fuzzy {

FuzzyPlayer::FuzzyPlayer(environm::soccer::clientEnvironm *environment, int debug_mode)
    : environment_(environment), debug_mode_(debug_mode) {
    readRulesFileElseDefault();
}

void FuzzyPlayer::readInputs() {
    for (std::tuple<std::string, std::string> input_definition : inputs_definition_) {
        std::string collection_name = std::get<0>(input_definition);
        std::string process_definition = std::get<1>(input_definition);
        float input_value = evaluateExpression(process_definition);
        collections_[collection_name]->setInput(input_value);
        std::cout << collection_name << " = " << input_value << std::endl;
        if (debug_mode_ > 0) {
            std::cout << collection_name << ":" << std::endl;
            collections_[collection_name]->printValues();
            std::cout << std::endl;
        }
    }
}

void FuzzyPlayer::process() {
    float spin = environment_->getSpin();

    float robot_angle = angle_system_->getOutput();
    float cosine = (float)cos(robot_angle*PI/180);
    float sine = (float)sin(robot_angle*PI/180);

    float robot_speed = speed_system_->getOutput();

    if (debug_mode_ > 0) {
        std::cout << "robot_angle:" << std::endl;
        collections_["robot_angle"]->setInput(robot_angle);
        collections_["robot_angle"]->printValues();
        std::cout << "\nrobot_speed:" << std::endl;
        collections_["robot_speed"]->setInput(robot_speed);
        collections_["robot_speed"]->printValues();
        std::cout << std::endl;
    }

    std::cout << "Robot angle -> " << robot_angle << std::endl;
    std::cout << "Robot speed -> " << robot_speed << std::endl;

    std::cout << "Robot spin = " << spin << std::endl;

    left_motor_ = robot_speed*(cosine - sine)/2;
    right_motor_ = robot_speed*(cosine + sine)/2;
    std::cout << "Left: " << left_motor_ << "\tRight: " << right_motor_ << std::endl;
}

bool FuzzyPlayer::act() {
    return environment_->act(left_motor_, right_motor_);
}

std::string getToken(std::string *str) {
    std::replace(str->begin(), str->end(), '\r', ' ');
    std::replace(str->begin(), str->end(), '\t', ' ');
    std::replace(str->begin(), str->end(), '=', ' ');
    std::string token = "";
    while (token == "" and *str != "") {
        token = str->substr(0, str->find(' '));
        *str = str->substr(str->find(' ')+1);
    }
    return token;
}

void FuzzyPlayer::readRulesFileElseDefault() {
    std::ifstream rules_file ("rules_description.txt");

    if (rules_file.is_open()) {
        readRulesFile(&rules_file);
        rules_file.close();
    } else {
        std::cout << "Couldn't open rules_description.txt. Loading default rules." << std::endl;
        std::cout << "Press any button to continue..." << std::endl;
        getchar();
        setRulesToDefault();
    }
}

void FuzzyPlayer::readRulesFile(std::ifstream *rules_file) {
    std::string line;
    std::string token;

    std::cout << "Reading rules description file." << std::endl;
    while (getline(*rules_file, line)) {
        token = getToken(&line);
        if (token == DOMAIN_TOKEN) {
            createDomainFromString(line);
        } else if (token == COLLECTION_TOKEN) {
            createCollectionFromString(line);
        } else if (token == SET_TOKEN) {
            createSetFromString(line);
        } else if (token == RULE_TOKEN) {
            createRuleFromString(line);
        } else if (token == INPUT_DEFINITION_TOKEN) {
            createInputDefinitionFromString(line);
        }
    }
}

void FuzzyPlayer::createDomainFromString(std::string domain_str) {
    std::string domain_name = getToken(&domain_str);
    float begin = evaluateExpression(getToken(&domain_str));
    float end = evaluateExpression(getToken(&domain_str));

    std::cout << "Creating domain: " << domain_name << "(" << begin << ", " << end << ")" << std::endl;
    domains_[domain_name] = new Domain(begin, end);
}

void FuzzyPlayer::createCollectionFromString(std::string collection_str) {
    std::string collection_name = getToken(&collection_str);
    std::string domain_name = getToken(&collection_str);

    std::cout << "Creating collection: " << collection_name << "(" << domain_name << ")" << std::endl;
    collections_[collection_name] = new FuzzyCollection(*domains_[domain_name]);

    if (collection_name == "robot_angle") {
        angle_system_ = new FuzzySystem(*domains_[domain_name]);
    }
    if (collection_name == "robot_speed") {
        speed_system_ = new FuzzySystem(*domains_[domain_name]);
    }
}

void FuzzyPlayer::createSetFromString(std::string set_str) {
    std::string collection_name = getToken(&set_str);
    std::string set_name = getToken(&set_str);
    FuzzySet set = FuzzySet();
    std::cout << "Creating set: " << collection_name << "(" << set_name << ") ";

    FuzzyCollection *collection = collections_[collection_name];

    std::string token = getToken(&set_str);
    float x = collection->getDomain().begin;
    float y = evaluateExpression(token);
    set.addPoint(x, y);
    std::cout << "(" << x << ',' << y << ") ";
    while (set_str != "") {
        token = getToken(&set_str);
        if (token.find(',') != token.npos) {
            std::replace(token.begin(), token.end(), '(', ' ');
            std::replace(token.begin(), token.end(), ')', ' ');
            std::replace(token.begin(), token.end(), ',', ' ');
            x = evaluateExpression(getToken(&token));
            y = evaluateExpression(getToken(&token));
        } else {
            x = collection->getDomain().end;
            y = evaluateExpression(token);
        }
        set.addPoint(x, y);
        std::cout << "(" << x << ',' << y << ") ";
    }
    std::cout << std::endl;
    collection->addSet(set, set_name);
}

void FuzzyPlayer::createInputDefinitionFromString(std::string input_definition_str) {
    std::string collection_name = getToken(&input_definition_str);
    std::string process_definition = getToken(&input_definition_str);
    std::tuple<std::string, std::string> to_push (collection_name, process_definition);
    inputs_definition_.push_back(to_push);
}

void FuzzyPlayer::createRuleFromString(std::string rule_str) {
    FuzzyRule rule = FuzzyRule();
    std::cout << "Creating rule: ";
    std::string token = "";
    while (token != "->") {
        std::string collection_name = getToken(&rule_str);
        std::string set_name = getToken(&rule_str);
        rule.addInput(collections_[collection_name], set_name);
        std::cout << collection_name << "(" << set_name << ") ";
        token = getToken(&rule_str);
    }
    std::string system_name = getToken(&rule_str);
    std::string set_name = getToken(&rule_str);
    rule.setOutput(collections_[system_name], set_name);
    if (system_name == "robot_angle") {
        angle_system_->addRule(rule);
    } else if (system_name == "robot_speed") {
        speed_system_->addRule(rule);
    }
    std::cout << "-> " << system_name << "(" << set_name << ")" << std::endl;
}

float FuzzyPlayer::evaluateExpression(std::string str) {
    if (str.find('+') != str.npos) {
        std::string left = str.substr(0, str.find('+'));
        std::string right = str.substr(str.find('+')+1);
        return evaluateExpression(left) + evaluateExpression(right);
    }
    if (str.find('-') != str.npos and str[0] != '-') {
        std::string left = str.substr(0, str.find('-'));
        std::string right = str.substr(str.find('-')+1);
        return evaluateExpression(left) - evaluateExpression(right);
    }
    if (str.find('*') != str.npos) {
        std::string left = str.substr(0, str.find('*'));
        std::string right = str.substr(str.find('*')+1);
        return evaluateExpression(left) * evaluateExpression(right);
    }
    if (str.find('/') != str.npos) {
        std::string left = str.substr(0, str.find('/'));
        std::string right = str.substr(str.find('/')+1);
        return evaluateExpression(left) / evaluateExpression(right);
    }
    return interpretValue(str);
}

float FuzzyPlayer::interpretValue(std::string str) {
    if (str == "BallAngle") {
        return environment_->getBallAngle();
    } else if (str == "BallDistance") {
        return environment_->getDistance();
    } else if (str == "ObstacleAngle") {
        return environment_->getObstacleAngle();
    } else if (str == "PI") {
        return PI;
    } else if (str == "MaxDistance") {
        // Environment gives height, width and goal deep as the double it actually is
        float height = environment_->getWorldHeight() / 2;
        // We also have to ignore the goals space in the width
        float width = (environment_->getWorldWidth() / 2) - environment_->getGoalDeep();
        return (float) sqrt(pow(height, 2) + pow(width, 2));
    } else if (str == "OwnScore") {
        return environment_->getOwnScore();
    } else if (str == "RivalScore") {
        return environment_->getRivalScore();
    } else if (str == "WorldWidth") {
        return environment_->getWorldWidth()/2;
    } else if (str == "WorldHeight") {
        return environment_->getWorldHeight()/2;
    } else if (str == "GoalDeep") {
        return environment_->getGoalDeep()/2;
    } else if (str == "GoalLength") {
        return environment_->getGoalLength()/2; // TODO check if goal length is also twice it should be
    } else if (str == "RobotRadius") {
        return environment_->getRobotRadius();
    } else if (str == "ObstacleCollision") {
        return environment_->getCollision();
    } else if (str == "RobotSpin") {
        return environment_->getSpin();
    } else if (str == "GoalAngle") {
        return environment_->getTargetAngle(environment_->getOwnGoal());
    } else if (str == "RivalGoalAngle") {
        return environment_->getTargetAngle(environment_->getRivalGoal());
    } else if (str == "RivalAngle") {
        return environment_->getTargetAngle(environment_->getRivalRobot().pos);
    } else if (str == "RivalDistance") {
        return (environment_->getRivalRobot().pos - environment_->getOwnRobot().pos).size();
    } else if (str == "MaxSpeed") {
        return MAX_SPEED;
    } else {
        return std::stof(str);
    }
}

void FuzzyPlayer::setRulesToDefault() {

    // Domains
    Domain angle = Domain(-180, 180);
    Domain speed = Domain(0, 1);

    angle_system_ = new FuzzySystem(angle);
    speed_system_ = new FuzzySystem(speed);

    // Ball angle
    collections_["ball_angle"] = new FuzzyCollection(angle);
    collections_["ball_angle"]->addSet(createDownLine(-180, 0, angle), "right");
    collections_["ball_angle"]->addSet(createTriangle(-90, 0, 90, angle), "front");
    collections_["ball_angle"]->addSet(createUpLine(0, 180, angle), "left");

    // Goal angle
    collections_["goal_angle"] = new FuzzyCollection(angle);
    collections_["goal_angle"]->addSet(createDownLine(-180, 0, angle), "right");
    collections_["goal_angle"]->addSet(createTriangle(-90, 0, 90, angle), "front");
    collections_["goal_angle"]->addSet(createUpLine(0, 180, angle), "left");

    // Robot angle
    collections_["robot_angle"] = new FuzzyCollection(angle);
    collections_["robot_angle"]->addSet(createDownLine(-180, 0, angle), "right");
    collections_["robot_angle"]->addSet(createTriangle(-180, 0, 180, angle), "front");
    collections_["robot_angle"]->addSet(createUpLine(0, 180, angle), "left");

    // Robot speed
    collections_["robot_speed"] = new FuzzyCollection(speed);
    collections_["robot_speed"]->addSet(createDownLine(0, 1, speed), "slow");
    collections_["robot_speed"]->addSet(createUpLine(0, 1, speed), "fast");

    // Rules
    {   // bf, gf -> rf
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "front");
        rule.addInput(collections_["goal_angle"], "front");
        rule.setOutput(collections_["robot_angle"], "front");
        angle_system_->addRule(rule);
    }
    {   // bl, gf -> rl
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "left");
        rule.addInput(collections_["goal_angle"], "front");
        rule.setOutput(collections_["robot_angle"], "left");
        angle_system_->addRule(rule);
    }
    {   // br, gf -> rr
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "right");
        rule.addInput(collections_["goal_angle"], "front");
        rule.setOutput(collections_["robot_angle"], "right");
        angle_system_->addRule(rule);
    }
    {   // bf, gl -> rr
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "front");
        rule.addInput(collections_["goal_angle"], "left");
        rule.setOutput(collections_["robot_angle"], "right");
        angle_system_->addRule(rule);
    }
    {   // bl, gl -> rl
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "left");
        rule.addInput(collections_["goal_angle"], "left");
        rule.setOutput(collections_["robot_angle"], "left");
        angle_system_->addRule(rule);
    }
    {   // br, gl -> rr
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "right");
        rule.addInput(collections_["goal_angle"], "left");
        rule.setOutput(collections_["robot_angle"], "right");
        angle_system_->addRule(rule);
    }
    {   // bf, gr -> rl
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "front");
        rule.addInput(collections_["goal_angle"], "right");
        rule.setOutput(collections_["robot_angle"], "left");
        angle_system_->addRule(rule);
    }
    {   // bl, gr -> rl
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "left");
        rule.addInput(collections_["goal_angle"], "right");
        rule.setOutput(collections_["robot_angle"], "left");
        angle_system_->addRule(rule);
    }
    {   // br, gr -> rr
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["ball_angle"], "right");
        rule.addInput(collections_["goal_angle"], "right");
        rule.setOutput(collections_["robot_angle"], "right");
        angle_system_->addRule(rule);
    }
    {
        FuzzyRule rule = FuzzyRule();
        rule.addInput(collections_["robot_speed"], "slow");
        rule.addInput(collections_["robot_speed"], "fast");
        rule.setOutput(collections_["robot_speed"], "fast");
        speed_system_->addRule(rule);
    }

    // Inputs definition
    std::tuple<std::string, std::string> ball_angle_process ("ball_angle", "BallAngle*180/PI");
    inputs_definition_.push_back(ball_angle_process);
    std::tuple<std::string, std::string> goal_angle_process ("goal_angle", "GoalAngle*180/PI");
    inputs_definition_.push_back(goal_angle_process);
}

} // namespace fuzzy
