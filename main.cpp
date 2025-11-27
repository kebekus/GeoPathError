
#include <QGeoPath>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/segment.hpp>

namespace bg = boost::geometry;

// This example creates a QGeoPath with one line segment and width 10.000m, and a point that is les than 3.500m from the path.
// The example verifies the distance with a computation using the boost standard library. However, Qt reports that the
// point is not contained in the path.

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    const QGeoPath qt_path( {{50, 7}, {52, 6.999}}, 10000);
    const QGeoCoordinate qt_point {51, 6.95};

    // Turn qt_path and qt_point into boost types
    using point_geo = bg::model::point<double,2, bg::cs::geographic<bg::degree>>; // Geographic (lon,lat) points in degrees
    const point_geo A( qt_path.path()[0].longitude(), qt_path.path()[0].latitude());
    const point_geo B( qt_path.path()[1].longitude(), qt_path.path()[1].latitude());
    const point_geo boost_point(qt_point.longitude(), qt_point.latitude());
    const bg::model::segment<point_geo> boost_path(A, B);

    // Use boost to compute the distance from boost_point to boost_path.
    // Using the Haversine strategy, which is an approximation with an error of 1% at most.
    constexpr double R = 6371000.0; // Mean Earth radius
    auto hav = bg::strategy::distance::haversine<double>(R);

    try
    {
        // The call does everything: projection, clamping, endpoint fallback.
        const double distance_m = bg::distance(boost_point, boost_path, hav);

        qWarning() << "Distance from qt_point to qt_path = " << distance_m << "m";
        qWarning() << "qt_path.contains( qt_point ):" << qt_path.contains( qt_point );
    }
    catch(...)
    {
        qWarning() << "Error";
    }

    return 0;
}
