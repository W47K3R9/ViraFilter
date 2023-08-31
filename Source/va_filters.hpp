//
//  va_filters.hpp
//  va_filters
//
//  Created by Lucas Scheidt on 29.08.23.
//

#ifndef va_filters_hpp
#define va_filters_hpp

#include "va_components.hpp"
#include "va_ports_impl.h"
#include <cmath>

class VAButterworth3
{
public:
    VAButterworth3() {}
    void initializeFilter(double _samplerate);
    double processSample(double _sample);
    void updateComponents(double frq, double res);
    ~VAButterworth3() {}
private:
    IdealVS source;
    Capacitor C1;
    Capacitor C3;
    Inductor L2;
    Resistor Rin;
    Resistor Rout;
    LeafWaveElement* leafs[3] = { &C1, &C3, &L2 };
    ParallelThreePort<Capacitor, Resistor>
        parallel_2 {C3, Rout};
    SerialThreePort<Inductor, decltype(parallel_2)>
        serial_2 {L2, parallel_2};
    ParallelThreePort<Capacitor, decltype(serial_2)>
        parallel_1 {C1, serial_2};
    SerialThreePort<Resistor, decltype(parallel_1)>
        serial_1 {Rin, parallel_1};
};

class VALPF2
{
public:
    VALPF2() {}
    void initializeFilter(double _samplerate);
    double processSample(double _sample);
    void updateComponents(double frq, double res);
    ~VALPF2() {}
private:
    IdealVS source;
    Resistor R1;
    Inductor L2;
    Capacitor C3;
    LeafWaveElement* leafs[2] = { &L2, &C3 };
    SerialThreePort<Inductor, Capacitor>
        serial_2 {L2, C3};
    SerialThreePort<Resistor, decltype(serial_2)>
        serial_1 {R1, serial_2};
};

#endif /* va_filters_hpp */
