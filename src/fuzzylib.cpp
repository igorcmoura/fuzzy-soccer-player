//
// Created by elRubo on 07/09/2016.
//

#include <iostream>
#include <climits>
#include "fuzzylib.h"

float trapezium_angle(int x, int a, int b, int c, int d){
    float y;

    if(x<=a || x>=d){     //out of range
        y =0;
    }else{
        if(x>=b && x<=c){ //middle of trapezium
            y = 1;
        }else{
            if(x>a && x<b){     //left ramp
                y = ((float)(x-a))/(b-a);
            }
            else{
                if(x>c && x<d){//right ramp
                    y = 1 - ((float)(x-c))/(d-c);
                }
            }
        }
    }

    return y;
}

float triangle_angle(int x, int a, int b, int c){
    return trapezium_angle(x, a, b, b, c);
}

float linear_up_angle(int x, int a, int b){
    return trapezium_angle(x, a, b, INT_MAX, INT_MAX);
}

float linear_down_angle(int x, int a, int b){
    return trapezium_angle(x, INT_MIN, INT_MIN, a, b);
}

std::vector<std::vector<float>> dot_product(std::vector<std::vector<float>> matrix_a, std::vector<std::vector<float>> matrix_b){

    //parameters validation:
    unsigned long int n_a_lines, n_b_lines, n_a_columns, n_b_columns;
    n_a_lines = matrix_a.size();
    n_b_lines = matrix_b.size();
    if(n_a_lines == 0 || n_b_lines == 0){
        std::cout << "Error! Matrix number of lines is zero." << std::endl;
        throw std::invalid_argument("Error! Matrix number of lines is zero.");
    }
    n_a_columns = matrix_a[0].size();
    n_b_columns = matrix_b[0].size();
    if(n_a_columns == 0 || n_b_columns == 0){
        std::cout << "Error! Matrix number of columns is zero." << std::endl;
        throw std::invalid_argument("Error! Matrix number of columns is zero.");
    }
    if(n_a_columns != n_b_lines){
        std::cout << "Error! These matrices can't be multiplied." << std::endl;
        throw std::invalid_argument( "Error! These matrices can't be multiplied.");
    }

    //b transposed         -> another function?
    std::vector<std::vector<float>> matrix_b_trans;
    for (int i = 0; i < n_b_columns; ++i) {
        std::vector<float> tmp;
        for (int j = 0; j < n_b_lines; ++j) {
            tmp.push_back(matrix_b[j][i]);
        }
        matrix_b_trans.push_back(tmp);
    }
    ///--------TEST-------- TO BE ERASED -------
    int n_b_trans_lines = matrix_b_trans.size();
    int n_b_trans_columns = matrix_b_trans[0].size();
    if(n_b_columns != n_b_trans_lines){
        std::cout << "Error! n_b_columns != n_btrans_line." << std::endl;

        throw std::invalid_argument("Error! n_b_columns != n_btrans_line.");
    }
    if(n_b_lines != n_b_trans_columns){
        std::cout << "Error! n_b_lines != n_btrans_columns." << std::endl;
        throw std::invalid_argument("Error! n_b_lines != n_btrans_columns.");
    }
    ///--------TEST-------- TO BE ERASED -------

    //dot product
    std::vector<std::vector<float>> final;
    for(std::vector<float> a_line : matrix_a) {   //for every line in matrix a
        std::vector<float> final_row;
        for (std::vector <float> b_trans_line : matrix_b_trans) { //for every line in b_trans (column in b)
            float max_min = std::min(a_line[0], b_trans_line[0]);
            for (int i = 1; i < n_a_columns; ++i) {
                max_min = std::max(max_min, (float) std::min(a_line[i], b_trans_line[i]));
            }
            final_row.push_back(max_min);
        }
        final.push_back(final_row);
    }
    return final;
}