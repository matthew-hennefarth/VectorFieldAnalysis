//
// Created by mhennefarth on 11/18/19.
//
#include <fstream>
#include <system_error>
#include <iostream>

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
    //We have read everything in, so now we can begin working on transforming all of the vectors to the new basis set (while retaining the old basis)

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