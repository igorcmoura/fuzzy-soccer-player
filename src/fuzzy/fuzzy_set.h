#ifndef FUZZYSOCCERPLAYER_FUZZYSET_H
#define FUZZYSOCCERPLAYER_FUZZYSET_H

#include <vector>

namespace fuzzy {

class Point {
public:
    Point(float x, float y) : x(x), y(y) {};
    Point(Point *point) : x(point->x), y(point->y) {};
    float x, y;

    bool equals(Point other);
};

class Line {
public:
    Line(Point p1, Point p2) : p1(p1), p2(p2) {};
    Point p1, p2;

    float findValue(float position);
    float findPosition(float value);
    Point *findIntersection(Line other);

private:
    bool between(Point point);
    bool hasAPointInCommon(Line other);
    Point *getCommonPoint(Line other);
};

class FuzzySet {
public:
    FuzzySet() {};
    FuzzySet(std::vector<Point> points);

    void addPoint(Point point);
    void addPoint(float position, float value);
    std::vector<Point> getPoints();

    float getValue(float value);
    FuzzySet cutAt(float alpha);
    FuzzySet join(FuzzySet other);
    float defuzzy();

private:
    std::vector<Point> points_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZYSET_H
