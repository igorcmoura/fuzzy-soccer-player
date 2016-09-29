#ifndef FUZZYSOCCERPLAYER_FUZZY_COLLECTION_H
#define FUZZYSOCCERPLAYER_FUZZY_COLLECTION_H

#include <map>
#include <string>

#import "fuzzy_set.h"

namespace fuzzy {

class FuzzyCollection {
public:
    FuzzyCollection(Domain domain);

    void addSet(FuzzySet set, std::string name);
    FuzzySet getSet(std::string name);
    float getValue(std::string set_name);
    void setInput(float value);

private:
    std::map<std::string, FuzzySet> sets_;
    float input_;
};

} // namespace fuzzy

#endif //FUZZYSOCCERPLAYER_FUZZY_COLLECTION_H
