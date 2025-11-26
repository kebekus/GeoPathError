
#include <QGeoPath>

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    const QGeoPath path( {{50, 7}, {52, 6.999}}, 10000);
    const QGeoCoordinate user1 {51, 6.95};
    const QGeoCoordinate user2 {51, 7.05};

    qWarning() << path.toString() << user1 << user2;
    qWarning() << "Is user1 near path: " << path.contains( user1 );
    qWarning() << "Is user2 near path: " << path.contains( user2 );

    return 0;
}
