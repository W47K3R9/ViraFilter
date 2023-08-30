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
    parallel_2.setConnections(&C3, &Rout);
    serial_2.setConnections(&L2, &parallel_2);
    parallel_1.setConnections(&C1, &serial_2);
    serial_1.setConnections(&Rin, &parallel_1);
    updateComponents(16e-3, 50);
}

void VAButterworth3::updateComponents(double frq, double res)
{
    C1.updateComponent(0.0002 * frq);
    C3.updateComponent(0.0002 * frq);
    L2.updateComponent(frq);
    Rin.updateComponent(50);
    Rout.updateComponent(res);
    for (NodeWaveElement* node : nodes)
    {
        node->updatePort();
    }
}

double VAButterworth3::processSample(double _sample)
{
    source.processSample(_sample);
    source.receiveIncidentWave(serial_1.emitReflectedWave());
    serial_1.receiveIncidentWave(source.emitReflectedWave());
    return virtualVoltage(&Rout);
}

void VALPF2::initializeFilter(double _samplerate)
{
    for (LeafWaveElement* leaf : leafs)
    {
        leaf->setSampleRate(_samplerate);
    }
    serial_2.setConnections(&L2, &C3);
    serial_1.setConnections(&R1, &serial_2);
    updateComponents(1000.0, 1.0);
}

void VALPF2::updateComponents(double frq, double res)
{
    C3.updateComponent(1);
    double induct_val = 1 / (4 * M_PI * M_PI * frq * frq);
    L2.updateComponent(induct_val);
    double res_val = std::sqrt(induct_val) / res;
    R1.updateComponent(res_val);
    for (NodeWaveElement* node : nodes)
    {
        node->updatePort();
    }
}

double VALPF2::processSample(double _sample)
{
    source.processSample(_sample);
    source.receiveIncidentWave(serial_1.emitReflectedWave());
    serial_1.receiveIncidentWave(source.emitReflectedWave());
    return virtualVoltage(&C3);
}
