//
//  wave_elements.cpp
//  va_dev_2
//
//  Created by Lucas Scheidt on 21.04.23.
//

#include "wave_elements.hpp"

/*
 ----- NodeWaveElement -----
 Set all connections and save the sub resistances.
 */

NodeWaveElement::NodeWaveElement(WaveElement* _cmp1, WaveElement* _cmp2)
: cmp1{_cmp1}, cmp2{_cmp2}
{
    if (cmp1 && cmp2 != nullptr)
    {
        updatePort();
    }
}

void NodeWaveElement::updatePort()
{
    sub_res[R1] = cmp1->port_res;
    sub_res[R2] = cmp2->port_res;
    sub_res[RSer] = sub_res[R1] + sub_res[R2];
    sub_res[R1Div] = sub_res[R1] / sub_res[RSer];
    sub_res[R2Div] = sub_res[R2] / sub_res[RSer];
    calcPortResistance();
}

void NodeWaveElement::setConnections(WaveElement *_cmp1, WaveElement *_cmp2)
{
    cmp1 = {_cmp1};
    cmp2 = {_cmp2};
    updatePort();
}

/*
 ----- SerialThreePort -----
 */

SerialThreePort::SerialThreePort(WaveElement* _cmp1, WaveElement* _cmp2)
: NodeWaveElement {_cmp1, _cmp2} {}

void SerialThreePort::receiveIncidentWave(double a)
{
    double reflected1 = - sub_res[R1Div] * (cmp2->ref_wave + a)
        + sub_res[R2Div] * cmp1->ref_wave;
    double reflected2 = - sub_res[R2Div] * (cmp1->ref_wave + a)
        + sub_res[R1Div] * cmp2->ref_wave;
    cmp1->receiveIncidentWave(reflected1);
    cmp2->receiveIncidentWave(reflected2);
    inc_wave = a;
}

double SerialThreePort::emitReflectedWave()
{
    ref_wave = - (cmp1->emitReflectedWave() + cmp2->emitReflectedWave());
    return ref_wave;
}

void SerialThreePort::calcPortResistance()
{
    port_res = sub_res[RSer];
}

/*
 ----- ParallelThreePort -----
 */

ParallelThreePort::ParallelThreePort(WaveElement* _cmp1, WaveElement* _cmp2)
: NodeWaveElement {_cmp1, _cmp2} {}

void ParallelThreePort::receiveIncidentWave(double a)
{
    double reflected1 = a - sub_res[R1Div]
        * (cmp1->ref_wave + cmp2->ref_wave);
    double reflected2 = a + sub_res[R2Div]
        * (cmp1->ref_wave - cmp2->ref_wave);
    cmp1->receiveIncidentWave(reflected1);
    cmp2->receiveIncidentWave(reflected2);
    inc_wave = a;
}

double ParallelThreePort::emitReflectedWave()
{
    ref_wave = sub_res[R2Div] * cmp1->emitReflectedWave()
        + sub_res[R1Div] * cmp2->emitReflectedWave();
    return ref_wave;
}

void ParallelThreePort::calcPortResistance()
{
    port_res = sub_res[R1] * sub_res[R2] / sub_res[RSer];
}

/*
 ----- LeafWaveElement -----
 */

void LeafWaveElement::updateComponent(double& _cmp_val)
{
    cmp_val = _cmp_val;
    calcPortResistance();
}

void LeafWaveElement::updateComponent(double&& _cmp_val)
{
    cmp_val = _cmp_val;
    calcPortResistance();
}

/*
 ----- Inductor ------
 */

double Inductor::emitReflectedWave()
{
    double sample_dly = ref_wave;
    ref_wave = inc_wave;
    return sample_dly;
}

void Inductor::calcPortResistance()
{
    port_res = 2 * samplerate * cmp_val;
}

/*
 ----- Capacitor -----
 */

double Capacitor::emitReflectedWave()
{
    double sample_dly = ref_wave;
    ref_wave = inc_wave;
    return sample_dly;
}

void Capacitor::calcPortResistance()
{
    port_res = 1 / (2 * samplerate * cmp_val);
}

/*
 ----- ResistiveVS -----
 */

void ResistiveVS::setConnection(WaveElement* _cmp)
{
    cmp = _cmp;
    calcPortResistance();
}
