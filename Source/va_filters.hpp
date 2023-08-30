//
//  va_filters.hpp
//  va_filters
//
//  Created by Lucas Scheidt on 29.08.23.
//

#ifndef va_filters_hpp
#define va_filters_hpp

#include "wave_elements.hpp"
#include <cmath>

class IVATopology
{
public:
    virtual void initializeFilter(double _samplerate) = 0;
    virtual double processSample(double _sample) = 0;
    virtual ~IVATopology(){}
};

class VAButterworth3 : public IVATopology
{
public:
    VAButterworth3() {}
    void initializeFilter(double _samplerate) override;
    double processSample(double _sample) override;
    void updateComponents(double frq, double res);
    ~VAButterworth3() {}
private:
    IdealVS source;
    ParallelThreePort parallel_2;
    SerialThreePort serial_2;
    ParallelThreePort parallel_1;
    SerialThreePort serial_1;
    NodeWaveElement* nodes[4] = { &parallel_2,
                                  &serial_2,
                                  &parallel_1,
                                  &serial_1 };
    Capacitor C1;
    Capacitor C3;
    Inductor L2;
    Resistor Rin;
    Resistor Rout;
    LeafWaveElement* leafs[5] = { &C1, &C3, &L2, &Rin, &Rout };
};

class VALPF2 : public IVATopology
{
public:
    VALPF2() {}
    void initializeFilter(double _samplerate) override;
    double processSample(double _sample) override;
    void updateComponents(double frq, double res);
    ~VALPF2() {}
private:
    IdealVS source;
    SerialThreePort serial_1;
    SerialThreePort serial_2;
    NodeWaveElement* nodes[2] = { &serial_2,
                                  &serial_1 };
    Resistor R1;
    Inductor L2;
    Capacitor C3;
    LeafWaveElement* leafs[3] = {&R1, &L2, &C3};
};

#endif /* va_filters_hpp */
