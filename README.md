# Fuzzy Soccer Player
Soccer player that uses fuzzy logic. This soccer player was made for the Neural Networks and Fuzzy Systems class at UFRGS.

[Work definition (in portuguese)](http://www.inf.ufrgs.br/~ewbasso/works/fuzsoccer/fuzsoccer_pt.htm)

## Rules definition
The rules for the soccer player are defined in the `rules_description.txt` file. They follow these conventions:
### Domain
A _domain_ is the range of values of a fuzzy set input.  
The domains are defined using:
```
d <domain name> <domain begin> <domain end>
```
`domain name` is a string without spaces representing the name of the domain;  
`domain begin` is an [expression](#expressions) representing the value of the beginning of the domain;  
`domain end` is an [expression](#expressions) representing the value of the end of the domain.  

E.g.:
```
d angle -180 180
```
Defines a domain named _angle_ that begins at -180 and ends in 180.
### Collection
A _collection_ is a group of fuzzy sets with an input in common. For example: ball left, ball front and ball right belong to the same collection because they have the same input: the ball angle.  
The collections are defined using:
```
c <collection name> <domain>
```
`collection name` is a string without spaces representing the name of the collection;  
`domain` is the name of an already defined [domain](#domain).  

E.g.:
```
c ball_angle angle
```
Defines a collection named _ball_angle_ and it's domain is the _angle_ domain.
### Set
A _set_ is a fuzzy set, it belongs to a collection, so it has the same domain as the collection. It contain points that are linearly connected to form the pertinence function.  
The sets are defines using:
```
s <collection> <set name> <begin value> [<point1> [<point2> ...]] <end value>
```
`collection` is the name of an already defined [collection](#collection);  
`set name` is a string without spaces representing the name of the set;  
`begin value` is a real number between 0 and 1 representing the value of the point at the beginning of the domain;  
`point#` is a pair `(<position>,<value>)` enclosed by parenthesis and separated by comma, being the position an [expression](#expressions) and the value a real number between 0 and 1. The points must be ordered by position;  
`end value` is a real number between 0 and 1 representing the value of the point at the end of the domain.  
The points are linearly connected.  

E.g.:
```
s ball_angle front 0 (-90,0) (0,1) (90,0) 0
```
Defines a set for the collection _ball_angle_ named _front_. It begins at 0 and is constant until the position -90, then it rises to one in the position 0, falls to 0 in the positision 90 and keeps constant until the end.
### Rule
A _rule_ is a fuzzy rule containing input sets and an output set.  
```
r <input collection1> <input set1> [&& <input collection2> <input set2> ...] -> <output collection> <output set>
```
`input collection#` and `output collection` are names of already defined [collections](#collection);  
`input set#` and `output set` are names of already defined [sets](#set).  

E.g.:
```
r ball_angle left && goal_angle front -> robot_angle left
```
Defines a rule for when the _ball_angle_ is _left_ and the _goal_angle_ is _front_, the _robot_angle_ in this case will be _left_.
### Input
An _input_ is the value of each collection input. It will be update for each game step.
```
i <collection> <value>
```
`collection` is the name of an already defined [collection](#collection);  
`value` is an [expression](#expressions).  

E.g.:
```
i ball_angle BallAngle*180/PI
```
Defines an input for the _ball_angle_, it will be the _BallAngle_ in each instant converted to degrees.
### Expressions
The _expressions_ can be:  
`<real number>` e.g.: `3.5`  
`<environment variable>` e.g.: `BallAngle`  
`<expression> <operand> <expression>` e.g.: `2+BallAngle`  

The supported _operands_ are `+ - * /`.  

The available _environment variables_ are:

| Name | Description |
|:---:| --- |
| WorldWidth | The world width in millimeters. |
| WorldHeight | The world height in millimeters. |
| GoalDeep | The deepness of the goals in millimeters. |
| GoalLength | The length of the goals in millimeters. |
| MaxDistance | The diagonal distance in millimeters of the field. |
| BallAngle | The angle of the line between the player and the ball relative to the player in radians. |
| BallDistance | The distance of the ball from the player in millimeters. |
| GoalAngle | The angle between the line that passes through the player and the ball and the line that passes through the ball and the goal in radians. |
| RivalGoalAngle | The angle between the line that passes through the player and the ball and the line that passes through the ball and the rival's goal in radians. |
| ObstacleAngle | The angle of the nearest obstacle relative to the player in radians. |
| ObstacleCollision | The distance in milimeters of the player and the nearest obstacle. |
| OwnScore | The player's score. |
| RivalScore | The rival's score. |
| RobotRadius | The radius of the robots. |
| RobotSpin | The spin of the player. |
| RivalAngle | The angle of the rival in radians. |
| RivalDistance | The distance in millimeters between the player and the rival. |
| MaxSpeed | The maximum speed constant. |
| PI | The value of PI. |
