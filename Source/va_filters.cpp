//
//  va_filters.cpp
//  va_dev_2
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
    C1.updateComponent(0.1e-3);
    C3.updateComponent(0.1e-3);
    L2.updateComponent(0.2e-3);
    Rres.updateComponent(1);
    serial_2.setConnections(&Rres, &C3);
    serial_1.setConnections(&L2, &serial_2);
    parallel_1.setConnections(&C1, &serial_1);
}


double VAButterworth3::processSample(double _sample)
{
    source.processSample(_sample);
    source.receiveIncidentWave(parallel_1.emitReflectedWave());
    parallel_1.receiveIncidentWave(source.emitReflectedWave());
    return virtualVoltage(&C3);
}
