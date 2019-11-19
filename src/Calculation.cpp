//
// Created by mhennefarth on 11/19/19.
//

#include <iostream>

#include "Grid.h"
#include "Calculation.h"

Calculation::Calculation() {

    Grid x ("efield.out");

    //Loop over total number of points
    for(int i = 0; i < 100000; i++){
        auto len = x.randomLength();
        auto start = x.randomPoint();
        auto initialCurvature = x.curvature(start);
        auto prev = start;
        auto prev_2 = start;
        auto prev_3 = start;
        //Bad initial guess, start again
        if(initialCurvature == -1){
            i--;
            continue;
        }
        for(unsigned int j = 0; j < len; j++){
            auto next = x.findNextPosition(prev);
            //This means we can't make a next move!
            if(next == std::array<int, 3>{-1 ,-1 ,-1}){
                break;
            }
            prev_3 = prev_2;
            prev_2 = prev;
            prev = next;
            //And we continue going!
        }

        auto finalCurvature = x.curvature(prev);
        if(finalCurvature == -1){
            prev = prev_2;
            finalCurvature = x.curvature(prev);
            if(finalCurvature == -1){
                prev = prev_3;
                finalCurvature = x.curvature(prev);
            }
        }
        double averageCurvature = (finalCurvature + initialCurvature) / 2.0;
        double distance = (x.at(start)[0] - x.at(prev)[0]).norm();
        std::cout << "Distance: " << distance << "\tCurvature: " << averageCurvature << '\n';
    }
}