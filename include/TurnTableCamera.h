#ifndef TURNTABLECAMERA_H
#define TURNTABLECAMERA_H

#include "Camera.h"

class TurnTableCamera : public Camera
{
    Q_OBJECT
    Q_PROPERTY(double phi READ phi WRITE setPhi NOTIFY phiChanged)
    Q_PROPERTY(double theta READ theta WRITE setTheta NOTIFY thetaChanged)
    Q_PROPERTY(double radius READ radius WRITE setRadius NOTIFY radiusChanged)
public:
    TurnTableCamera(QObject* parent=0);

    double phi() const;
    double theta() const;
    double radius() const;

public slots:
    void setPhi(double phi);
    void setTheta(double theta);
    void setRadius(double radius);

signals:
    void phiChanged(double);
    void thetaChanged(double);
    void radiusChanged(double);

private:
    void updateViewMatrix();

private:
    double _phi;
    double _theta;
    double _radius;
};

#endif // TURNTABLECAMERA_H
