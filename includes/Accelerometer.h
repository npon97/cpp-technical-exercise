#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

class Accelerometer {
public:
    virtual ~Accelerometer() {}

    virtual void initialize() = 0;
    virtual void readAcceleration() = 0;
    virtual double getX() const = 0;
    virtual double getY() const = 0;
    virtual double getZ() const = 0;
};

#endif /* ACCELEROMETER_H_ */