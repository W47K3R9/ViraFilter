//
//  va_filters.hpp
//  va_dev_2
//
//  Created by Lucas Scheidt on 29.08.23.
//

#ifndef va_filters_hpp
#define va_filters_hpp

#include "wave_elements.hpp"

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
    ~VAButterworth3() {}
private:
    RootVS source;
    ParallelThreePort parallel_1;
    SerialThreePort serial_1;
    SerialThreePort serial_2;
    Capacitor C1;
    Capacitor C3;
    Inductor L2;
    Resistor Rres;
    LeafWaveElement* leafs[4] = {&C1, &C3, &L2, &Rres};
};

class VAButterNew : public IVATopology
{
public:
    VAButterNew() {}
    void initializeFilter(double _samplerate) override;
    double processSample(double _sample) override;
    ~VAButterNew() {}
private:
    RootVS source;
    ParallelThreePort parallel_1;
    SerialThreePort serial_1;
    Capacitor C1;
    Capacitor C3;
    Inductor L2;
    LeafWaveElement* leafs[3] = {&C1, &C3, &L2};
};
#endif /* va_filters_hpp */
