#include "fuzzy_set.h"

namespace fuzzy {

float FuzzySet::fuzzifie(float value) {
    float output = 0;
    Point last_point = points_[0];
    for (int i = 1; i < points_.size(); i++) {
        Point current_point = points_[i];
        if (value >= last_point.x and value <= current_point.x)
            float last_point_weight = last_point.y*(current_point.x - value);
            float
            float new_value = last_point.y + (current_point.y - last_point.y)*(value - last_point.x)/(current_point.x - last_point.x);
            output = std::max(output, )
    }

    if (value >= high_1_ and value <= high_2_)
        return 1.0;
    else if (value > low_1_ and value < high_1_)
        return (value - low_1_)/(high_1_ - low_1_);
    else if (value > high_2_ and value < low_2_)
        return (low_2_ - value)/(low_2_ - high_2_);
    else
        return 0.0;
}

void FuzzySet::addPoint(Point point) {
    points_.push_back(point);
}

void FuzzySet::addPoint(float position, float value) {
    addPoint(Point(position, value));
}

} // namespace fuzzy
