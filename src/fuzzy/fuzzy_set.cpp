#include "fuzzy_set.h"

namespace fuzzy {

FuzzySet::FuzzySet(std::vector<Point> points) : points_(points) {}

float FuzzySet::fuzzifie(float value) {
    float output = 0;
    Point last_point = points_[0];
    for (int i = 1; i < points_.size(); i++) {
        Point current_point = points_[i];
        if (value >= last_point.x and value <= current_point.x) {
            float last_point_weight = last_point.y * (current_point.x - value);
            float current_point_weight = current_point.y * (value - last_point.x);
            float new_value = (last_point_weight + current_point_weight) / (current_point.x - last_point.x);
            output = std::max(output, new_value);
        }
    }
    return output;
}

void FuzzySet::addPoint(Point point) {
    points_.push_back(point);
}

void FuzzySet::addPoint(float position, float value) {
    addPoint(Point(position, value));
}

} // namespace fuzzy
