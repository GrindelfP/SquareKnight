#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <stdexcept>

#define NORM_TOLERANCE 1e-9

namespace GVector {

    class Vector2D {

    public:
        double x;
        double y;

        Vector2D(double x, double y) {
            this->x = x;
            this->y = y;
        }

        Vector2D operator+(const Vector2D& B) const {
            return Vector2D(this->x + B.x, this->y + B.y);
        }

        Vector2D operator-(const Vector2D& B) const {
            return Vector2D(this->x - B.x, this->y - B.y);
        }

        Vector2D operator/(const double k) const {
            if (k == 0) throw std::runtime_error("Vector2D instance division by zero!");
            return *this * (1/k);
        }

        Vector2D operator*(const double k) const {
            return Vector2D(this->x * k, this->y * k);
        }

        inline void inplaceMultiplyByReal(const double k) {
            this->x *= k;
            this->y *= k;
        }

        inline double dot(const Vector2D& B) const {
            return this->x * B.x + this->y * B.y;
        }

        inline double cross(const Vector2D& B) const {
            return this->x * B.y - this->y * B.x;
        }

        inline double norm() const {
            return sqrt(this->x * this->x + this->y * this->y);
        }

        inline Vector2D normalize() const {
            double vectorNorm = this->norm();
            if (vectorNorm < NORM_TOLERANCE) // this is possible if vec is very small or (0, 0)
                return Vector2D(0.0, 0.0); 
            return *this * (1.0 / vectorNorm);
        }
    };

    class Vector3D {

    public:
        double x;
        double y;
        double z;

        Vector3D(double x, double y, double z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        inline Vector3D operator+(const Vector3D& B) const {
            return Vector3D(this->x + B.x, this->y + B.y, this->z + B.z);
        }

        inline Vector3D operator-(const Vector3D& B) const {
            return Vector3D(this->x - B.x, this->y - B.y, this->z - B.z);
        }

        inline Vector3D operator/(const double k) const {
            if (k == 0) throw std::runtime_error("Vector3D instance division by zero!");
            return *this * (1/k);
        }

        inline Vector3D operator*(const double k) const {
            return Vector3D(this->x * k, this->y * k, this->z * k);
        }

        inline void inplaceMultiplyByReal(const double k) {
            this->x *= k;
            this->y *= k;
            this->z *= k;
        }

        inline double dot(const Vector3D& B) const {
            return this->x * B.x + this->y * B.y + this->z * B.z;
        }

        inline Vector3D cross(const Vector3D& B) const {
            return Vector3D(
                this->y * B.z - this->z * B.y, 
                this->z * B.x - this->x * B.z, 
                this->x * B.y - this->y * B.x);
        }

        inline double norm() const {
            return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        }

        inline Vector3D normalize() const {
            double vectorNorm = this->norm();
            if (vectorNorm < NORM_TOLERANCE) // this is possible if vec is very small or (0, 0, 0)
                return Vector3D(0.0, 0.0, 0.0); 
            return *this * (1.0 / vectorNorm);
        }
    };

    // SYMMETRY COMPANION FUNCTIONS
    inline Vector2D operator*(double k, const Vector2D& a) {
        return a * k;
    }
    
    inline Vector3D operator*(double k, const Vector3D& a) {
        return a * k;
    }
} // namespace GVector

#endif // VECTOR_H
