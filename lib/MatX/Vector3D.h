//
// Created by mhennefarth on 11/15/19.
//

#ifndef VECTORFIELDANALYSIS_VECTOR3D_H
#define VECTORFIELDANALYSIS_VECTOR3D_H

#include "BaseVector.h"

namespace mx {

    template<class type>
    class Vector3D : BaseVector<type, 3> {

    public:

        Vector3D cross(const Vector3D &vec) const;

    };

    template<class type>
    Vector3D<type> Vector3D<type>::cross(const Vector3D &vec) const {

        Vector3D<type> result({this->m_values[1] * vec.m_values[2] - this->m_values[2] * vec.m_values[1],
                               this->m_values[2] * vec.m_values[0] - this->m_values[0] * vec.m_values[2],
                               this->m_values[0] * vec.m_values[1] - this->m_values[1] * vec.m_values[0]
                              });
        return result;
    }

    typedef Vector3D<double> Vec3d;
    typedef Vector3D<float> Vec3f;
    typedef Vector3D<int> Vec3i;
}
#endif //VECTORFIELDANALYSIS_VECTOR3D_H
