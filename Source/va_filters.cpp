//
//  va_filters.cpp
//  va_filters
//
//  Created by Lucas Scheidt on 29.08.23.
//

#include "va_filters.hpp"

void VAButterworth3::initializeFilter(double _samplerate)
{
    for (LeafWaveElement* leaf : leafs)
    {
        leaf->setSampleRate(_samplerate);
    }
    updateComponents(16e-3, 50);
}

void VAButterworth3::updateComponents(double frq, double res)
{
    C1.calcPortResistance(0.0002 * frq);
    C3.calcPortResistance(0.0002 * frq);
    L2.calcPortResistance(frq);
    Rin.calcPortResistance(50);
    Rout.calcPortResistance(res);
    parallel_2.updatePort();
    serial_2.updatePort();
    parallel_1.updatePort();
    serial_1.updatePort();
}

double VAButterworth3::processSample(double _sample)
{
    source.processSample(_sample);
    source.receiveIncidentWave(serial_1.emitReflectedWave());
    serial_1.receiveIncidentWave(source.emitReflectedWave());
    return virtualVoltage<Resistor>(Rout);
}

void VALPF2::initializeFilter(double _samplerate)
{
    for (LeafWaveElement* leaf : leafs)
    {
        leaf->setSampleRate(_samplerate);
    }
    updateComponents(1000.0, 1.0);
}

void VALPF2::updateComponents(double frq, double res)
{
    C3.calcPortResistance(1.0);
    double induct_val = 1 / (4 * M_PI * M_PI * frq * frq);
    L2.calcPortResistance(induct_val);
    double res_val = std::sqrt(induct_val) / res;
    R1.calcPortResistance(res_val);
    serial_2.updatePort();
    serial_1.updatePort();
}

double VALPF2::processSample(double _sample)
{
    source.processSample(_sample);
    source.receiveIncidentWave(serial_1.emitReflectedWave());
    serial_1.receiveIncidentWave(source.emitReflectedWave());
    return virtualVoltage<Capacitor>(C3);
}
