//
// Created by elRubo on 07/09/2016.
//

#ifndef SEMNOME_FUZZYLIB_H
#define SEMNOME_FUZZYLIB_H

#include <vector>

float trapezium_angle(int x, int a, int b, int c, int d);
float triangle_angle(int x, int a, int b, int c);
float linear_up_angle(int x, int a, int b);
float linear_down_angle(int x, int a, int b);
std::vector<std::vector<float>> dot_product(std::vector<std::vector<float>> matrix_a, std::vector<std::vector<float>> matrix_b);

#endif //SEMNOME_FUZZYLIB_H
