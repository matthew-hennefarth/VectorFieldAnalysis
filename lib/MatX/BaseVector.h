//
// Created by mhennefarth on 11/15/19.
//

#ifndef VECTORFIELDANALYSIS_BASEVECTOR_H
#define VECTORFIELDANALYSIS_BASEVECTOR_H

#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

namespace mx {

    template<class type=double, unsigned int size = 1>
    class BaseVector {
    public:
        // Ensures that we only have numeric types being created
        static_assert(std::is_arithmetic<type>::value, "Type must be numeric");
        static_assert(size > 0, "Size of a vector must be a positive number");

        BaseVector();

        BaseVector(const std::vector<type> &vec);

        BaseVector(const type vec[size]);

        BaseVector(const type &val);

        BaseVector(const BaseVector &copy);


        unsigned int dim() const;

        double norm() const;

        BaseVector &normalize();

        type dot(const BaseVector &vec) const;

        BaseVector<type, size + 1> raise_dimension() const;

        BaseVector<type, size - 1> lower_dimension() const;

        //Returns the vec projected onto *this vector.
        BaseVector<type, size> project(const BaseVector &vec) const;

        //Returns in radians
        double angleBetween(const BaseVector &vec) const;

        template<unsigned int dim>
        BaseVector<type, dim> cast_dimension() const;

        //Overload operators
        const type &operator[](const int &index) const;

        type &operator[](const unsigned int &index);

        //Additions
        BaseVector operator+(const BaseVector &x) const;

        BaseVector operator+(const type &x) const;

        BaseVector &operator+=(const BaseVector &x);

        BaseVector &operator+=(const type &x);

        //Multiplications
        BaseVector operator*(const BaseVector &x) const;

        BaseVector operator*(const type &x) const;

        BaseVector &operator*=(const BaseVector &x);

        BaseVector &operator*=(const type &x);

        //Subtractions
        BaseVector operator-(const BaseVector &x) const;

        BaseVector operator-(const type &x) const;

        BaseVector &operator-=(const BaseVector &x);

        BaseVector &operator-=(const type &x);


        //Divisions
        BaseVector operator/(const BaseVector &x) const;

        BaseVector operator/(const type &x) const;

        BaseVector &operator/=(const BaseVector &x);

        BaseVector &operator/=(const type &x);

        //Assignments
        BaseVector &operator=(const BaseVector &x);

        BaseVector &operator=(const type vec[size]);

        //Booleans
        bool operator==(const BaseVector &x) const;

        bool operator!=(const BaseVector &x) const;

        bool operator>(const BaseVector &x) const;

        bool operator>=(const BaseVector &x) const;

        bool operator<(const BaseVector &x) const;

        bool operator<=(const BaseVector &x) const;

    protected:
        unsigned int m_size = size;

    private:
        type m_values[size];

    };

    template<class type, unsigned int size>
    BaseVector<type, size>::BaseVector() {
        //Create a vector of 0s
        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] = 0;
        }
    }

    template<class type, unsigned int size>
    BaseVector<type, size>::BaseVector(const std::vector<type> &vec) {
        assert(m_size == vec.size());
        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] = vec[i];
        }
    }

    template<class type, unsigned int size>
    BaseVector<type, size>::BaseVector(const BaseVector<type, size> &copy) {
        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] = copy.m_values[i];
        }
    }

    template<class type, unsigned int size>
    BaseVector<type, size>::BaseVector(const type vec[size]) : m_values(vec) {}

    template<class type, unsigned int size>
    BaseVector<type, size>::BaseVector(const type &val) {
        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] = val;
        }
    }

    template<class type, unsigned int size>
    unsigned int BaseVector<type, size>::dim() const {

        return m_size;

    }

    template<class type, unsigned int size>
    double BaseVector<type, size>::norm() const {
        type sum = 0;
        for (unsigned int i = 0; i < m_size; i++) {
            sum += m_values[i] * m_values[i];
        }

        assert(sum >= 0);
        return sqrt(sum);
    }


    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::normalize() {
        type norm = this->norm();
        for (unsigned int i = 0; i < m_size; i++) {
            m_values /= norm;
        }

        return *this;
    }


    template<class type, unsigned int size>
    type BaseVector<type, size>::dot(const BaseVector<type, size> &vec) const {
        type sum = 0;
        for (unsigned int i = 0; i < m_size; i++) {
            sum += m_values[i] * vec.m_values[i];
        }

        return sum;
    }

    template<class type, unsigned int size>
    const type &BaseVector<type, size>::operator[](const int &index) const {
        assert((index >= (m_size * -1) && index < m_size && index != 0, "Index Out of Bounds"));
        if(index > 0){
            return m_values[index];
        }
        else {
            return m_values[m_size - index];
        }
    }

    template<class type, unsigned int size>
    type &BaseVector<type, size>::operator[](const unsigned int &index) {
        //Type casts out
        return const_cast<type &>(static_cast<const BaseVector<type, size> &> (*this)[index]);
    }

    template<class type, unsigned int size>
    BaseVector<type, size + 1> BaseVector<type, size>::raise_dimension() const {
        BaseVector<type, size + 1> result;
        for (unsigned int i = 0; i < m_size; i++) {
            result[i] = m_values[i];
        }
        return result;
    }

    template<class type, unsigned int size>
    BaseVector<type, size - 1> BaseVector<type, size>::lower_dimension() const {
        BaseVector<type, size - 1> result;
        for (unsigned int i = 0; i < m_size - 1; i++) {
            result[i] = m_values[i];
        }
        return result;
    }

    template<class type, unsigned int size>
    template<unsigned int dim>
    BaseVector<type, dim> BaseVector<type, size>::cast_dimension() const {

        BaseVector<type, dim> result;
        for (unsigned int i = 0; i < result.dim() && i < m_size; i++) {
            result[i] = m_values[i];
        }

        return result;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::project(const BaseVector<type, size> &vec) const {

        BaseVector<type, size> result;

        result = (*this) * (this->dot(vec) / (this->norm()));

        return result;

    }

    template<class type, unsigned int size>
    double BaseVector<type, size>::angleBetween(const BaseVector<type, size> &vec) const {

        return acos((this->dot(vec)) / (this->norm() * vec.norm()));

    }

    //Addition Overloaded Functions
    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator+(const BaseVector<type, size> &x) const {

        BaseVector<type, size> result;
        for (unsigned int i = 0; i < m_size; i++) {
            result.m_values[i] = m_values[i] + x.m_values[i];
        }

        return result;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator+(const type &x) const {

        BaseVector<type, size> result;

        for (unsigned int i = 0; i < m_size; i++) {
            result.m_values[i] = m_values[i] + x;
        }

        return result;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator+=(const BaseVector<type, size> &x) {

        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] += x.m_values[i];
        }

        return *this;
    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator+=(const type &x) {

        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] -= x;
        }
        return *this;
    }

    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator*(const BaseVector<type, size> &x) const {

        BaseVector<type, size> result;
        for (unsigned int i = 0; i < m_size; i++) {
            result.m_values[i] = m_values[i] * x.m_values[i];
        }

        return result;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator*(const type &x) const {

        BaseVector<type, size> result;
        for (unsigned int i = 0; i < m_size; i++) {
            result.m_values[i] = m_values[i] * x;
        }

        return result;
    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator*=(const BaseVector<type, size> &x) {

        for (unsigned int i = 0; i < m_size; i++) {

            m_values[i] *= x.m_values[i];

        }

        return *this;
    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator*=(const type &x) {

        for (unsigned int i = 0; i < m_size; i++) {

            m_values[i] *= x;

        }

        return *this;
    }

    //Subtraction Overloaded Functions
    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator-(const BaseVector<type, size> &x) const {

        return (*this) + (x * (-1));

    }

    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator-(const type &x) const {
        return (*this) + (x * (-1));
    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator-=(const BaseVector<type, size> &x) {

        return this += (x * -1);

    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator-=(const type &x) {
        return this += (x * -1);
    }

    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator/(const BaseVector<type, size> &x) const {

        BaseVector<type, size> result;

        for (unsigned int i = 0; i < m_size; i++) {

            result.m_values[i] = m_values[i] / x.m_values[i];

        }

        return result;
    }

    template<class type, unsigned int size>
    BaseVector<type, size> BaseVector<type, size>::operator/(const type &x) const {

        return this->operator*(static_cast<type>(1 / x));

    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator/=(const BaseVector<type, size> &x) {

        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] /= x.m_values[i];
        }
        return *this;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator/=(const type &x) {

        return this->operator*=(static_cast<type>(1 / x));

    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator=(const BaseVector<type, size> &x) {

        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] = x.m_values[i];
        }
        return *this;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> &BaseVector<type, size>::operator=(const type vec[size]) {

        for (unsigned int i = 0; i < m_size; i++) {
            m_values[i] = vec[i];
        }
        return *this;

    }

    template<class type, unsigned int size>
    bool BaseVector<type, size>::operator==(const BaseVector<type, size> &x) const {
        for (unsigned int i = 0; i < m_size; i++) {
            if (m_values[i] != x.m_values[i])
                return false;
        }

        return true;
    }

    template<class type, unsigned int size>
    bool BaseVector<type, size>::operator!=(const BaseVector<type, size> &x) const {

        return !((*this) == x);

    }

    template<class type, unsigned int size>
    bool BaseVector<type, size>::operator>(const BaseVector<type, size> &x) const {

        for (unsigned int i = 0; i < m_size; i++) {
            if (m_values[i] <= x.m_values[i])
                return false;
        }

        return true;

    }

    template<class type, unsigned int size>
    bool BaseVector<type, size>::operator>=(const BaseVector<type, size> &x) const {

        return !((*this) < x);

    }

    template<class type, unsigned int size>
    bool BaseVector<type, size>::operator<(const BaseVector<type, size> &x) const {

        for (unsigned int i = 0; i < m_size; i++) {
            if (m_values[i] >= x.m_values[i])
                return false;
        }

        return true;

    }

    template<class type, unsigned int size>
    bool BaseVector<type, size>::operator<=(const BaseVector<type, size> &x) const {
        return !((*this) > x);
    }

    // Functions for allowing 6 * vector or 6 + vector...while not mathematically rigorous, they are sometimes useful
    template<class type, unsigned int size>
    BaseVector<type, size> operator*(const type &x, const BaseVector<type, size> &vec){

        return vec * x;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> operator+(const type &x, const BaseVector<type, size> &vec){

        return vec + x;

    }

    template<class type, unsigned int size>
    BaseVector<type, size> operator-(const type &x, const BaseVector<type, size> &vec){

        return -1* (vec - x);

    }

    template<class type, unsigned int size>
    BaseVector<type, size> operator/(const type &x, const BaseVector<type, size> &vec){

        BaseVector<type, size> result;

        for(unsigned int i = 0; i < vec.dim(); i++){
            result[i] = x / vec[i];
        }

        return result;

    }

    template<class type, unsigned int size>
    std::ostream& operator<<(std::ostream& os, const BaseVector<type, size> &obj){

        os << '[';
        for(unsigned int i = 0; i < obj.dim()-1; i++){
            os << obj[i] << ", ";
        }
        os << obj[obj.dim()-1] << ']';

        return os;

    }

}

#endif //VECTORFIELDANALYSIS_BASEVECTOR_H
