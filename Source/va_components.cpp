//
//  va_components.cpp
//  ViraFilter
//
//  Created by Lucas Scheidt on 31.08.23.
//  Copyright © 2023 LeadBackToSines. All rights reserved.
//

#include "va_components.hpp"

/*
 ----- Resistor ------
 */

void Resistor::calcPortResistance(double& _cmp_val)
{
    cmp_val = _cmp_val;
    port_res = _cmp_val;
}

void Resistor::calcPortResistance(double&& _cmp_val)
{
    cmp_val = _cmp_val;
    port_res = _cmp_val;
}

/*
 ----- Inductor ------
 */

double Inductor::emitReflectedWave()
{
    ref_wave = - inc_wave;
    return ref_wave;
}

void Inductor::calcPortResistance(double& _cmp_val)
{
    cmp_val = _cmp_val;
    port_res = 2 * samplerate * cmp_val;
}

void Inductor::calcPortResistance(double&& _cmp_val)
{
    cmp_val = _cmp_val;
    port_res = 2 * samplerate * cmp_val;
}

/*
 ----- Capacitor -----
 */

double Capacitor::emitReflectedWave()
{
    ref_wave = inc_wave;
    return ref_wave;
}

void Capacitor::calcPortResistance(double& _cmp_val)
{
    cmp_val = _cmp_val;
    port_res = 1 / (2 * samplerate * cmp_val);
}

void Capacitor::calcPortResistance(double&& _cmp_val)
{
    cmp_val = _cmp_val;
    port_res = 1 / (2 * samplerate * cmp_val);
}
