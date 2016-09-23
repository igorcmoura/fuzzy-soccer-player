#include "fuzzy_set.h"
#define DEFUZZY_STEPS 15

namespace fuzzy {

FuzzySet::FuzzySet(std::vector<Point> points) : points_(points) {}

float FuzzySet::getValue(float value) {
    float output = 0;
    Point last_point = points_.front();
    for (int i = 1; i < points_.size(); i++) {
        Point current_point = points_[i];
        if (value >= last_point.x and value <= current_point.x) {
            Line line = Line(last_point, current_point);
            float new_value = line.findValue(value);
            output = std::max(output, new_value);
        }
        last_point = current_point;
    }
    return output;
}

void FuzzySet::addPoint(Point point) {
    if (points_.empty() or !point.equals(points_.back()))
        points_.push_back(point);
}

void FuzzySet::addPoint(float position, float value) {
    addPoint(Point(position, value));
}

std::vector<Point> FuzzySet::getPoints() {
    return points_;
}

FuzzySet FuzzySet::cutAt(float alpha) {
    FuzzySet new_set = FuzzySet();

    Point last_point = points_.front();
    if (last_point.y > alpha)
        new_set.addPoint(last_point.x, alpha);
    else
        new_set.addPoint(last_point);

    for (int i = 1; i < points_.size(); i++) {
        Point current_point = points_[i];
        if (last_point.y <= alpha and current_point.y <= alpha)
            new_set.addPoint(current_point);
        else if ((last_point.y > alpha) != (current_point.y > alpha)) {
            Line line = Line(last_point, current_point);
            float position = line.findPosition(alpha);
            new_set.addPoint(position, alpha);
        }

        last_point = current_point;
    }

    if (points_.back().y > alpha)
        new_set.addPoint(points_.back().x, alpha);
    else
        new_set.addPoint(points_.back());

    return new_set;
}

FuzzySet FuzzySet::join(FuzzySet other) {
    FuzzySet new_set = FuzzySet();

    bool this_is_higher;
    std::vector<Point> other_points = other.getPoints();
    if (points_.front().y > other_points.front().y) {
        new_set.addPoint(points_.front());
        this_is_higher = true;
    } else {
        new_set.addPoint(other_points.front());
        this_is_higher = false;
    }

    int this_index = 1;
    int other_index = 1;

    while (this_index < (points_.size()-1) or other_index < (other_points.size()-1)) {
        Line this_line = Line(points_[this_index-1], points_[this_index]);
        Line other_line = Line(other_points[other_index-1], other_points[other_index]);
        Point* intersection = this_line.findIntersection(other_line);
        if (intersection != nullptr) {
            new_set.addPoint(*intersection);
            this_is_higher = !this_is_higher;
        }
        if (points_[this_index].x < other_points[other_index].x) {
            if (this_is_higher)
                new_set.addPoint(points_[this_index]);
            this_index++;
        } else {
            if (!this_is_higher)
                new_set.addPoint(other_points[other_index]);
            other_index++;
        }
    }

    if (points_.back().y > other_points.back().y)
        new_set.addPoint(points_.back());
    else
        new_set.addPoint(other_points.back());

    return new_set;
}

float FuzzySet::defuzzy() {
    float accumulator =0, den_accum = 0, temp;
    float limit = points_.back().x;
    float step = (limit - points_.front().x)/DEFUZZY_STEPS;
    for(float i =0; i< limit; i += step){
        temp = getValue(i);
        accumulator += i*temp;
        den_accum += temp;
    }
    return accumulator/den_accum;
}

float Line::findValue(float position) {
    float a_weight = p1.y * (p2.x - position);
    float b_weight = p2.y * (position - p1.x);
    return (a_weight + b_weight) / (p2.x - p1.x);
}

float Line::findPosition(float value) {
    Point inverted_p1 = Point(p1.y, p1.x);
    Point inverted_p2 = Point(p2.y, p2.x);
    return Line(inverted_p1, inverted_p2).findValue(value);
}

Point *Line::findIntersection(Line other) {
    if (hasAPointInCommon(other))
        return getCommonPoint(other);

    float x = (other.p1.x*(p1.x*p2.y + other.p2.y*(-p1.x+p2.x) - p2.x*p1.y) + other.p2.x*(-p1.x*p2.y + other.p1.y*(p1.x - p2.x) + p2.x*p1.y))/(other.p1.y*(p1.x-p2.x)+other.p2.y*(-p1.x+p2.x)+(other.p1.x-other.p2.x)*(p2.y-p1.y));
    float y = (x-p1.x)*(p2.y-p1.y)/(p2.x-p1.x) + p1.y;

    Point* point = new Point(x, y);
    if (!between(*point) or !other.between(*point))
        return nullptr;
    else
        return point;
}

bool Line::between(Point point) {
    float x_uplimit = std::max(p1.x, p2.x);
    float x_downlimit = std::min(p1.x, p2.x);
    bool between_x = point.x < x_uplimit and point.x > x_downlimit;

    float y_uplimit = std::max(p1.y, p2.y);
    float y_downlimit = std::min(p1.y, p2.y);
    bool between_y = point.y < y_uplimit and point.y > y_downlimit;

    return between_x and between_y;
}

bool Line::hasAPointInCommon(Line other) {
    return  p1.equals(other.p1) or
            p1.equals(other.p2) or
            p2.equals(other.p1) or
            p2.equals(other.p2);
}

Point *Line::getCommonPoint(Line other) {
    if (p1.equals(other.p1) or p1.equals(other.p2))
        return new Point(&p1);
    else if (p2.equals(other.p1) or p2.equals(other.p2))
        return new Point(&p2);
    return nullptr;
}

    bool Point::equals(Point other) {
    return  (x == other.x and y == other.y);
}

} // namespace fuzzy
