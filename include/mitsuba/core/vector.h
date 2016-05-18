#pragma once

#include <mitsuba/mitsuba.h>

#if !defined(NDEBUG)
#  define EIGEN_INITIALIZE_MATRICES_BY_NAN 1
#endif

#define EIGEN_DONT_PARALLELIZE 1

#include <Eigen/Core>
#include <Eigen/Geometry>

NAMESPACE_BEGIN(mitsuba)

/* ===================================================================
    This file contains a few templates and specializations, which
    provide 2/3D points, vectors, and normals over different
    underlying data types. Points, vectors, and normals are distinct
    in Nori, because they transform differently under homogeneous
    coordinate transformations.
 * =================================================================== */

/**
 * \brief Generic N-dimensional vector data structure based on Eigen::Matrix
 */
template <typename _Scalar, int _Dimension> struct TVector : public Eigen::Matrix<_Scalar, _Dimension, 1> {
public:
    enum {
        Dimension = _Dimension
    };

    typedef _Scalar                             Scalar;
    typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
    typedef TVector<Scalar, Dimension>          VectorType;
    typedef TPoint<Scalar, Dimension>           PointType;

    /// Create a new vector with constant component vlaues
    TVector(Scalar value = (Scalar) 0) { Base::setConstant(value); }

    /// Create a new 2D vector (type error if \c Dimension != 2)
    TVector(Scalar x, Scalar y) : Base(x, y) { }

    /// Create a new 3D vector (type error if \c Dimension != 3)
    TVector(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

    /// Create a new 4D vector (type error if \c Dimension != 4)
    TVector(Scalar x, Scalar y, Scalar z, Scalar w) : Base(x, y, z, w) { }

    /// Construct a vector from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> TVector(const Eigen::MatrixBase<Derived>& p)
        : Base(p) { }

    /// Assign a vector from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> TVector &operator=(const Eigen::MatrixBase<Derived>& p) {
        this->Base::operator=(p);
        return *this;
    }
};

/**
 * \brief Generic N-dimensional point data structure based on Eigen::Matrix
 */
template <typename _Scalar, int _Dimension> struct TPoint : public Eigen::Matrix<_Scalar, _Dimension, 1> {
public:
    enum {
        Dimension = _Dimension
    };

    typedef _Scalar                             Scalar;
    typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
    typedef TVector<Scalar, Dimension>          VectorType;
    typedef TPoint<Scalar, Dimension>           PointType;

    /// Create a new point with constant component vlaues
    TPoint(Scalar value = (Scalar) 0) { Base::setConstant(value); }

    /// Create a new 2D point (type error if \c Dimension != 2)
    TPoint(Scalar x, Scalar y) : Base(x, y) { }

    /// Create a new 3D point (type error if \c Dimension != 3)
    TPoint(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

    /// Create a new 4D point (type error if \c Dimension != 4)
    TPoint(Scalar x, Scalar y, Scalar z, Scalar w) : Base(x, y, z, w) { }

    /// Construct a point from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> TPoint(const Eigen::MatrixBase<Derived>& p)
        : Base(p) { }

    /// Assign a point from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> TPoint &operator=(const Eigen::MatrixBase<Derived>& p) {
        this->Base::operator=(p);
        return *this;
    }
};

/**
 * \brief 3-dimensional surface normal representation
 */
struct Normal3f : public Eigen::Matrix<Float, 3, 1> {
public:
    enum {
        Dimension = 3
    };

    typedef float                               Scalar;
    typedef Eigen::Matrix<Scalar, Dimension, 1> Base;
    typedef TVector<Scalar, Dimension>          VectorType;
    typedef TPoint<Scalar, Dimension>           PointType;


    /// Create a new normal with constant component vlaues
    Normal3f(Scalar value = 0.0f) { Base::setConstant(value); }

    /// Create a new 3D normal
    Normal3f(Scalar x, Scalar y, Scalar z) : Base(x, y, z) { }

    /// Construct a normal from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> Normal3f(const Eigen::MatrixBase<Derived>& p)
        : Base(p) { }

    /// Assign a normal from MatrixBase (needed to play nice with Eigen)
    template <typename Derived> Normal3f &operator=(const Eigen::MatrixBase<Derived>& p) {
        this->Base::operator=(p);
        return *this;
    }
};

/// Complete the set {a} to an orthonormal base {a, b, c}
extern MTS_EXPORT_CORE void coordinateSystem(const Vector3f &a, Vector3f &b, Vector3f &c);

template <typename Derived> std::ostream& operator<<(std::ostream &os, const Eigen::MatrixBase<Derived>& m) {
    constexpr bool isColVector = Eigen::MatrixBase<Derived>::ColsAtCompileTime == 1;
    if (isColVector)
        os << m.transpose().format(Eigen::IOFormat(4, isColVector ? Eigen::DontAlignCols : 0, ", ", ";\n", "", "", "[", "]"));
    else
        os << m.format(Eigen::IOFormat(4, isColVector ? Eigen::DontAlignCols : 0, ", ", ";\n", "", "", "[", "]"));
    return os;
}

template <typename Derived> std::ostream& operator<<(std::ostream &os, const Eigen::ArrayBase<Derived>& a) {
    os << a.matrix();
    return os;
}


NAMESPACE_END(mitsuba)

NAMESPACE_BEGIN(Eigen)
NAMESPACE_BEGIN(internal)
template <typename Scalar, int Dimension>
struct traits<mitsuba::TVector<Scalar, Dimension>>
    : public traits<typename mitsuba::TVector<Scalar, Dimension>::Base> { };

template <typename Scalar, int Dimension>
struct traits<mitsuba::TPoint<Scalar, Dimension>>
    : public traits<typename mitsuba::TPoint<Scalar, Dimension>::Base> { };

template <> struct traits<mitsuba::Normal3f> : public traits<mitsuba::Normal3f::Base> { };
NAMESPACE_END(internal)
NAMESPACE_END(Eigen)