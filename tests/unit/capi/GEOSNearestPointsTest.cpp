// $Id: GEOSNearestPointsTest.cpp 2424 2009-04-29 23:52:36Z mloskot $
//
// Test Suite for C-API GEOSNearestPoints

#include <tut/tut.hpp>
// geos
#include <geos_c.h>
// std
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace tut {
//
// Test Group
//

// Common data used in test cases.
struct test_capigeosnearestpoints_data {
    GEOSGeometry* geom1_;
    GEOSGeometry* geom2_;

    static void
    notice(const char* fmt, ...)
    {
        std::fprintf(stdout, "NOTICE: ");

        va_list ap;
        va_start(ap, fmt);
        std::vfprintf(stdout, fmt, ap);
        va_end(ap);

        std::fprintf(stdout, "\n");
    }

    test_capigeosnearestpoints_data()
        : geom1_(nullptr), geom2_(nullptr)
    {
        initGEOS(notice, notice);
    }

    ~test_capigeosnearestpoints_data()
    {
        GEOSGeom_destroy(geom1_);
        GEOSGeom_destroy(geom2_);
        geom1_ = nullptr;
        geom2_ = nullptr;
        finishGEOS();
    }

    void checkNearestPoints(const char* wkt1, const char* wkt2,
                            double x1, double y1,
                            double x2, double y2)
    {
        geom1_ = GEOSGeomFromWKT(wkt1);
        ensure(nullptr != geom1_);
        geom2_ = GEOSGeomFromWKT(wkt2);
        ensure(nullptr != geom2_);
        GEOSCoordSequence* coords_ = GEOSNearestPoints(geom1_, geom2_);

        unsigned int size;
        GEOSCoordSeq_getSize(coords_, &size);
        ensure_equals(size, 2u);

        double  ox, oy;

        /* Point in geom1_ */
        GEOSCoordSeq_getOrdinate(coords_, 0, 0, &ox);
        GEOSCoordSeq_getOrdinate(coords_, 0, 1, &oy);
        ensure_equals(ox, x1);
        ensure_equals(oy, y1);

        /* Point in geom2_ */
        GEOSCoordSeq_getOrdinate(coords_, 1, 0, &ox);
        GEOSCoordSeq_getOrdinate(coords_, 1, 1, &oy);
        ensure_equals(ox, x2);
        ensure_equals(oy, y2);

        GEOSCoordSeq_destroy(coords_);
    }

    void checkNearestPointsNull(const char* wkt1, const char* wkt2)
    {
        geom1_ = GEOSGeomFromWKT(wkt1);
        ensure(nullptr != geom1_);
        geom2_ = GEOSGeomFromWKT(wkt2);
        ensure(nullptr != geom2_);
        GEOSCoordSequence* coords_ = GEOSNearestPoints(geom1_, geom2_);

        ensure(nullptr == coords_);
    }

};

typedef test_group<test_capigeosnearestpoints_data> group;
typedef group::object object;

group test_capigeosnearestpoints_group("capi::GEOSNearestPoints");

//
// Test Cases
//

template<>
template<>
void object::test<1>
()
{
    checkNearestPointsNull("POLYGON EMPTY", "POLYGON EMPTY");
}

template<>
template<>
void object::test<2>
()
{
    checkNearestPoints(
        "POLYGON((1 1,1 5,5 5,5 1,1 1))",
        "POLYGON((8 8, 9 9, 9 10, 8 8))",
        5, 5, 8, 8
    );

}

template<>
template<>
void object::test<3>
()
{
    checkNearestPoints(
        "POLYGON((1 1,1 5,5 5,5 1,1 1))",
        "POINT(2 2)",
        2, 2, 2, 2
    );

}

template<>
template<>
void object::test<4>
()
{
    checkNearestPoints(
        "LINESTRING(1 5,5 5,5 1,1 1)",
        "POINT(2 2)",
        2, 1, 2, 2
    );
}

template<>
template<>
void object::test<5>
()
{
    checkNearestPoints(
        "LINESTRING(0 0,10 10)",
        "LINESTRING(0 10,10 0)",
        5, 5, 5, 5
    );
}

} // namespace tut

