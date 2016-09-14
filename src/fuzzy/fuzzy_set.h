#ifndef FUZZYSOCCERPLAYER_FUZZYSET_H
#define FUZZYSOCCERPLAYER_FUZZYSET_H

#include <vector>

#include "fuzzy.h"

namespace fuzzy {

class Point {
public:
    Point(float x, float y) : x(x), y(y) {};
    float x, y;
};

class FuzzySet {
public:
    FuzzySet() {};
    FuzzySet(std::vector<Point> points);

    void addPoint(Point point);
    void addPoint(float position, float value);
    float fuzzifie(float value);

private:
    std::vector<Point> points_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYSET_H
