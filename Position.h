#ifndef POSITION_H
#define POSITION_H

#include <QByteArray>

typedef struct Position {
    Position():
        x(0.0),
        y(0.0),
        theta(0.0)
    {};
    Position(float x, float y, float theta):
        x(x),
        y(y),
        theta(theta)
    {};
    float x;
    float y;
    float theta;

public:
    QByteArray serialize() {
        QByteArray response = QByteArray();
        response.append(((char*)&x)[0])
                .append(((char*)&x)[1])
                .append(((char*)&x)[2])
                .append(((char*)&x)[3]);

        response.append(((char*)&y)[0])
                .append(((char*)&y)[1])
                .append(((char*)&y)[2])
                .append(((char*)&y)[3]);

        response.append(((char*)&theta)[0])
                .append(((char*)&theta)[1])
                .append(((char*)&theta)[2])
                .append(((char*)&theta)[3]);

        return response;
    }

} Position;

#endif /* POSITION_H */
