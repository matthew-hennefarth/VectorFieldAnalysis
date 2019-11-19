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

    Eigen::Vector3d toBasis(const Eigen::Vector3d &vec) const;
    Eigen::Vector3d fromBasis(const Eigen::Vector3d &vec) const;
    Eigen::Vector3d toBasisPos(const Eigen::Vector3d &vec) const;
    Eigen::Vector3d fromBasisPos(const Eigen::Vector3d &vec) const;
    const std::array<Eigen::Vector3d, 2>& at(const unsigned int &x, const unsigned int &y, const unsigned int &z) const;
    std::array<Eigen::Vector3d, 2>& at(const unsigned int &x, const unsigned int &y, const unsigned int &z);
    const std::array<Eigen::Vector3d, 2>& at(const std::array<int, 3> &pos) const;
    std::array<Eigen::Vector3d, 2>& at(const std::array<int, 3> &pos);
    std::array<int, 3> findNextPosition(const int &x, const int &y, const int &z) const;
    std::array<int, 3> findNextPosition(const std::array<int, 3> &pos) const;

    //This returns the curvature at the next point actually!!!!!!!
    double curvature(const int &x, const int &y, const int &z) const;
    double curvature(const std::array<int, 3> &pos) const;

    std::array<int, 3> randomPoint() const;
    unsigned int randomLength() const;



private:
    //Functions
    static Eigen::Vector3d m_extractBasisVector(const std::string &fileLine);

    //Variables
    std::vector<std::vector<std::vector<std::array<Eigen::Vector3d, 2>>>> m_values;
    //[x,y,z] dimensions
    unsigned int m_dimensions[3];
    Eigen::Vector3d m_basis[3];
    double m_spacing[3];
    std::string m_fileName;
    // Given a vector, we will initially use the transformToBasis, but we may want to original position with the transformFromBasis
    Eigen::Matrix3d m_transformToBasis, m_transformFromBasis;
    Eigen::Vector3d m_center;

};


#endif //VECTORFIELDANALYSIS_GRID_H
