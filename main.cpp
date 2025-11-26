
#include <QGeoPath>

namespace {

double deg2rad(double deg)
{
    return deg * M_PI / 180.0;
}

double haversine(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371000.0; // Earth radius (meters)
    const double dLat = lat2 - lat1;
    const double dLon = lon2 - lon1;
    const double a = sin(dLat/2)*sin(dLat/2) + cos(lat1)*cos(lat2)*sin(dLon/2)*sin(dLon/2);
    return 2 * R * atan2(sqrt(a), sqrt(1-a));
}

double bearing(double lat1, double lon1, double lat2, double lon2)
{
    const double y = sin(lon2-lon1) * cos(lat2);
    const double x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lon2-lon1);
    return atan2(y, x);
}

double distancePointToSegment(const QGeoCoordinate& A, const QGeoCoordinate& B, const QGeoCoordinate& P)
{
    const double R = 6371000.0;

    const double lat1 = deg2rad(A.latitude());
    const double lon1 = deg2rad(A.longitude());
    const double lat2 = deg2rad(B.latitude());
    const double lon2 = deg2rad(B.longitude());
    const double latP = deg2rad(P.latitude());
    const double lonP = deg2rad(P.longitude());

    const double d13 = haversine(lat1, lon1, latP, lonP) / R;
    const double d12 = haversine(lat1, lon1, lat2, lon2) / R;
    const double brg13 = bearing(lat1, lon1, latP, lonP);
    const double brg12 = bearing(lat1, lon1, lat2, lon2);

    const double dxt = asin(sin(d13) * sin(brg13 - brg12)); // radians

    // Check if projection lies outside the segment
    const double dat = acos( cos(d13) / cos(dxt) );  // along-track distance
    if (dat < 0)
        return haversine(latP, lonP, lat1, lon1); // closest to A
    if (dat > d12)
        return haversine(latP, lonP, lat2, lon2); // closest to B

    return fabs(dxt) * R; // perpendicular distance
}

} // namespace


int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    const QGeoPath path1( {{50, 7}, {52, 6.999}}, 10000);
    const QGeoPath path2( {{50, 7}, {52, 7}}, 10000);
    const QGeoCoordinate user1 {51, 6.95};
    const QGeoCoordinate user2 {51, 7.05};

    qWarning() << "Is user1 near path1:" << path1.contains( user1 ) << "Distance is" << distancePointToSegment(path1.coordinateAt(0), path1.coordinateAt(1), user1) << "m";
    qWarning() << "Is user2 near path1:" << path1.contains( user2 ) << "Distance is" << distancePointToSegment(path1.coordinateAt(0), path1.coordinateAt(1), user2) << "m";
    qWarning() << "Is user1 near path2:" << path2.contains( user1 ) << "Distance is" << distancePointToSegment(path2.coordinateAt(0), path2.coordinateAt(1), user1) << "m";
    qWarning() << "Is user2 near path2:" << path2.contains( user2 ) << "Distance is" << distancePointToSegment(path2.coordinateAt(0), path2.coordinateAt(1), user2) << "m";

    return 0;
}
