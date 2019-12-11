//
// Created by mhennefarth on 11/18/19.
//
#include <fstream>
#include <system_error>
#include <iostream>
#include <cmath>
#include <cstdlib>

#include "Grid.h"
#include "Utilities.h"

Grid::Grid(const std::string &file) : m_fileName(file){

    std::ifstream inFile;

    try{
        inFile.open(file);
    }
    catch(std::system_error& e){
        std::cerr << e.code().message() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    try{
        if(inFile.is_open()){
            std::string fileLine;

            int x = 0, y = 0, z = 0;

            while(inFile.good()){
                getline(inFile, fileLine);

                if(contains(fileLine, "V1")){
                    m_basis[0] = m_extractBasisVector(fileLine);
                }
                else if(contains(fileLine, "V2")){
                    m_basis[1] = m_extractBasisVector(fileLine);
                }
                else if(contains(fileLine, "Normal")){
                    m_basis[2] = m_extractBasisVector(fileLine);
                }
                else if(contains(fileLine, "Center")){
                    m_center = m_extractBasisVector(fileLine);
                }
                else if(contains(fileLine, "Plane")){
                    auto line = split(fileLine, ':');
                    fileLine = line[1];

                    line = split(split(fileLine, ' ')[0], 'x');
                    try {
                        m_dimensions[0] = std::stoi(line[0]);
                    } catch(const std::invalid_argument &ia){
                        std::cerr << "Invalid argument: " << ia.what() << '\n';
                        std::exit(EXIT_FAILURE);
                    }

                    try {
                        m_dimensions[1] = std::stoi(line[1]);
                    } catch(const std::invalid_argument &ia){
                        std::cerr << "Invalid argument: " << ia.what() << '\n';
                        std::exit(EXIT_FAILURE);
                    }
                    // Grabs the spacing
                    m_spacing[0] = std::stod(split(fileLine, ' ').at(3));
                    m_spacing[1] = m_spacing[0];

                }
                else if(contains(fileLine, "Vert")){
                    auto line = split(fileLine, ' ');
                    try {
                        m_dimensions[2] = std::stoi(line.at(1)) + 1;
                        //There is actually 50+1 since we have to account for the ends (like 24 above and below, with one for the origin)
                    } catch(const std::invalid_argument &ia){
                        std::cerr << "Invalid argument: " << ia.what() << '\n';
                        std::exit(EXIT_FAILURE);

                    }
                    try {
                        m_spacing[2] = std::stod(line.at(4));
                    } catch(const std::invalid_argument &ia){
                        std::cerr << "Invalid argument: " << ia.what() << '\n';
                        std::exit(EXIT_FAILURE);
                    }
                    // Resize so that the outer is z (height), then y, and then x (which should be the same)
                    m_values.resize(m_dimensions[2], std::vector<std::vector<std::array<Eigen::Vector3d, 2>>>(m_dimensions[1], std::vector<std::array<Eigen::Vector3d, 2>> (m_dimensions[0])));
                    m_transformFromBasis << m_basis[0], m_basis[1], m_basis[2];
                    m_transformToBasis = m_transformFromBasis.inverse().eval();

                }
                else if(fileLine.size() > 3) {
                    //Now we handle all of the vectors
                    replace(fileLine, 'D', 'E', true);
                    auto line = split(fileLine, ' ');
                    assert(line.size() == 7);
                    try {
                        Eigen::Vector3d pos(std::stod(line.at(0)), std::stod(line.at(1)), std::stod(line.at(2)));
                        Eigen::Vector3d field(std::stod(line.at(3)), std::stod(line.at(4)), std::stod(line.at(5)));

                        //Instead of looping over everything again later, lets just begin the transformation now
                        pos = toBasisPos(pos);
                        field = toBasis(field);

                        std::array<Eigen::Vector3d, 2> vecs = {pos, field};
                        m_values.at(z).at(y).at(x) = vecs;
                    } catch (const std::invalid_argument &ia){
                        std::cerr << "Invalid argument: " << ia.what() << '\n';
                        std::exit(EXIT_FAILURE);
                    }
                    x++;
                    if (x == m_dimensions[0]){
                        x = 0;
                        y++;
                    }
                    if(y == m_dimensions[1]){
                        y = 0;
                        z++;
                    }
                }

            }
            inFile.close();
        }
        else{
            std::cerr << "Unable to open file" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } catch (std::system_error& e) {
        std::cerr << "End of file issue" << std::endl;
    }
}

Eigen::Vector3d Grid::toBasis(const Eigen::Vector3d &vec) const{
    return m_transformToBasis*vec;
}

Eigen::Vector3d Grid::fromBasis(const Eigen::Vector3d &vec) const{
    return m_transformFromBasis*vec;
}
Eigen::Vector3d Grid::toBasisPos(const Eigen::Vector3d &vec) const{
    return m_transformToBasis*(vec-m_center);
}
Eigen::Vector3d Grid::fromBasisPos(const Eigen::Vector3d &vec) const{
    return (m_transformFromBasis*vec) + m_center;
}


Eigen::Vector3d Grid::m_extractBasisVector(const std::string &fileLine){

    auto line = split(fileLine, '[').at(1);
    line = line.substr(0, line.size()-1);
    std::vector<std::string> str_vec = split(line, ' ');
    assert(str_vec.size() == 3);
    try {
        return {std::stod(str_vec.at(0)), std::stod(str_vec.at(1)), std::stod(str_vec.at(2))};
    } catch(const std::invalid_argument &ia){
        std::cerr << "Invalid argument: " << ia.what() << '\n';
        std::exit(EXIT_FAILURE);
    }
}

const std::array<Eigen::Vector3d, 2>& Grid::at(const unsigned int &x, const unsigned int &y, const unsigned int &z) const{
    return m_values.at(z).at(y).at(x);
}
std::array<Eigen::Vector3d, 2>& Grid::at(const unsigned int &x, const unsigned int &y, const unsigned int &z){

    return const_cast<std::array<Eigen::Vector3d, 2> &>  (static_cast<const Grid&> (*this).at(x,y,z));

}

const std::array<Eigen::Vector3d, 2>& Grid::at(const std::array<int, 3> &pos) const{
    return at(pos[0], pos[1], pos[2]);
}

std::array<Eigen::Vector3d, 2>& Grid::at(const std::array<int, 3> &pos){
    return at(pos[0], pos[1], pos[2]);
}


// Returns the x,y,z of the next cube that the "particle" would go into
// Returns -1 -1 -1 if there is no cube (ie it leaves the box)
std::array<int, 3> Grid::findNextPosition(const int &x, const int &y, const int &z) const{

    std::array<int, 3> position{-1 ,-1, -1};

    std::vector<std::pair<double, std::array<int, 3>>> possiblePositions;

    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            for(int k = -1; k < 2; k++){

                if(i == 0 && j == 0 && k == 0){
                    continue;
                }
                else if(x+i < 0 || x+i >= m_dimensions[0]){
                    continue;
                }
                else if(y+j < 0 || y+j >= m_dimensions[1]){
                    continue;
                }
                else if(z+k < 0 || z+k >= m_dimensions[2]){
                    continue;
                }
                else{
                    //Compute dot product between E/Field at x,y,z and position of (x+i, y+j, z+k) - (x,y,z)
                    auto r = at(x+i, y+j, z+k)[0] - at(x,y,z)[0];
                    auto field = at(x,y,z)[1];

                    auto angle = acos(r.dot(field) / (r.norm() * field.norm())) * 180/PI;
                    if(angle < 30){ // We will filter out anything that is not less than 60/2 degrees
                        possiblePositions.push_back({angle, {x+i, y+j, z+k}});
                    }
                }
            }
        }
    }

    if(possiblePositions.empty()){
        return position;
    }

    unsigned int minIndex = 0;

    for(unsigned int i = 1; i < possiblePositions.size(); i++){
        if(std::get<0>(possiblePositions[minIndex]) >= std::get<0>(possiblePositions[i])){
            minIndex = i;
        }
    }

    return std::get<1>(possiblePositions[minIndex]);
}
std::array<int, 3> Grid::findNextPosition(const std::array<int, 3> &pos) const{
    return findNextPosition(pos[0], pos[1], pos[2]);
}

//Returns -1 if there is an issue getting the curvature
//Else, returns the curvature
double Grid::curvature(const int &x, const int &y, const int &z) const{

    std::array<std::array<Eigen::Vector3d, 2>, 3> positions;
    positions[0] = at(x,y,z);
    auto next = findNextPosition(x,y,z);
    if(next == std::array<int, 3>{-1, -1, -1}){
        return -1;
    }
    positions[1] = at(next);
    next = findNextPosition(next);
    if(next == std::array<int, 3>{-1, -1, -1}){
        return -1;
    }
    positions[2] = at(next);

    //Positions has Ei-1, Ei, Ei+1 now...so we can theoretically compute the curvature slowly

    auto r_prime = positions[1][1];

    Eigen::Vector3d r_prime_prime = (positions[2][1] - positions[0][1]) / 2.0;

    //Now we find the curvature

    auto cross = r_prime.cross(r_prime_prime);

    return (cross.norm()/ (r_prime.norm()* r_prime.norm()*r_prime.norm()));

}

double Grid::curvature(const std::array<int, 3> &pos) const {
    return curvature(pos[0], pos[1], pos[2]);
}

std::array<int, 3> Grid::randomPoint() const{

    return {static_cast<int>(rand()%m_dimensions[0]),static_cast<int>(rand()%m_dimensions[1]), static_cast<int>(rand()%m_dimensions[2])};

}

unsigned int Grid::randomLength() const{

    double min_dimension = m_dimensions[0];
    for(int i = 0; i < 3; i++){
        if(min_dimension > m_dimensions[i]){
            min_dimension = m_dimensions[i];
        }
    }

    return static_cast<int>(rand()%(static_cast<int>(min_dimension/2))) + 2;
}