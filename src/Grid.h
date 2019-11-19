//
// Created by mhennefarth on 11/18/19.
//

#ifndef VECTORFIELDANALYSIS_GRID_H
#define VECTORFIELDANALYSIS_GRID_H

#include <string>
#include <vector>

#include "Dense"

class Grid {

public:
    Grid(const std::string &file);

//    Eigen::Vector3d& vectorAt(const unsigned int &x, const unsigned int &y, const unsigned int &z);
//    Eigen::Vector3d& vectorAt(const Eigen::Matrix<unsigned int, 3, 1> &vec)
//    Eigen::Vector3d& vectorPos(const unsigned int &x, const unsigned int &y, const unsigned int &z);
//    Eigen::Vector3d& vectorPos(const Eigen::Matrix<unsigned int, 3, 1> &vec);
//
//    std::vector<Eigen::Vector3d&> neighboringCells(const unsigned int &x, const unsigned int &y, const unsigned int &z);
//    std::vector<Eigen::Vector3d&> neighboringCells(const Eigen::Matrix<unsigned int, 3, 1> &vec);

    Eigen::Vector3d toBasis(const Eigen::Vector3d &vec) const;
    Eigen::Vector3d fromBasis(const Eigen::Vector3d &vec) const;
    Eigen::Vector3d toBasisPos(const Eigen::Vector3d &vec) const;
    Eigen::Vector3d fromBasisPos(const Eigen::Vector3d &vec) const;


private:
    //Functions
    static Eigen::Vector3d m_extractBasisVector(const std::string &fileLine);

    //Variables
    std::vector<std::vector<std::vector<std::array<Eigen::Vector3d, 2>>>> m_values;
    unsigned int m_dimensions[3];
    Eigen::Vector3d m_basis[3];
    double m_spacing[3];
    std::string m_fileName;
    // Given a vector, we will initiall use the transformToBasis, but we may want to original position with the transformFromBasis
    Eigen::Matrix3d m_transformToBasis, m_transformFromBasis;
    Eigen::Vector3d m_center;

};


#endif //VECTORFIELDANALYSIS_GRID_H
