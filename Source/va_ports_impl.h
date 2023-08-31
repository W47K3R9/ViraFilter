//
//  va_ports_impl.h
//  ViraFilter
//
//  Created by Lucas Scheidt on 31.08.23.
//  Copyright © 2023 LeadBackToSines. All rights reserved.
//

#ifndef va_ports_impl_h
#define va_ports_impl_h

#include "va_ports.h"

/*
 ----- SerialThreePort -----
 */

template <class CMP_TP_1, class CMP_TP_2>
SerialThreePort<CMP_TP_1, CMP_TP_2>
    ::SerialThreePort(CMP_TP_1& _cmp1, CMP_TP_2& _cmp2)
: cmp1 {_cmp1}, cmp2 {_cmp2} { updatePort(); }

template <class CMP_TP_1, class CMP_TP_2>
void SerialThreePort<CMP_TP_1, CMP_TP_2>::updatePort()
{
    sub_res[R1] = cmp1.port_res;
    sub_res[R2] = cmp2.port_res;
    sub_res[RSer] = sub_res[R1] + sub_res[R2];
    sub_res[R1Div] = sub_res[R1] / sub_res[RSer];
    sub_res[R2Div] = sub_res[R2] / sub_res[RSer];
    calcPortResistance();
}

template <class CMP_TP_1, class CMP_TP_2>
void SerialThreePort<CMP_TP_1, CMP_TP_2>
    ::receiveIncidentWave(double a)
{
    double reflected1 = - sub_res[R1Div] * (cmp2.ref_wave + a)
        + sub_res[R2Div] * cmp1.ref_wave;
    double reflected2 = - sub_res[R2Div] * (cmp1.ref_wave + a)
        + sub_res[R1Div] * cmp2.ref_wave;
    cmp1.receiveIncidentWave(reflected1);
    cmp2.receiveIncidentWave(reflected2);
    inc_wave = a;
}

template <class CMP_TP_1, class CMP_TP_2>
double SerialThreePort<CMP_TP_1, CMP_TP_2>::emitReflectedWave()
{
    ref_wave = - (cmp1.emitReflectedWave() + cmp2.emitReflectedWave());
    return ref_wave;
}

template <class CMP_TP_1, class CMP_TP_2>
void SerialThreePort<CMP_TP_1, CMP_TP_2>::calcPortResistance()
{
    port_res = sub_res[RSer];
}

/*
 ----- ParallelThreePort -----
 */

template <class CMP_TP_1, class CMP_TP_2>
ParallelThreePort<CMP_TP_1, CMP_TP_2>
    ::ParallelThreePort(CMP_TP_1& _cmp1, CMP_TP_2& _cmp2)
: cmp1 {_cmp1}, cmp2 {_cmp2} { updatePort(); }

template <class CMP_TP_1, class CMP_TP_2>
void ParallelThreePort<CMP_TP_1, CMP_TP_2>::updatePort()
{
    sub_res[R1] = cmp1.port_res;
    sub_res[R2] = cmp2.port_res;
    sub_res[RSer] = sub_res[R1] + sub_res[R2];
    sub_res[R1Div] = sub_res[R1] / sub_res[RSer];
    sub_res[R2Div] = sub_res[R2] / sub_res[RSer];
    calcPortResistance();
}

template <class CMP_TP_1, class CMP_TP_2>
void ParallelThreePort<CMP_TP_1, CMP_TP_2>::receiveIncidentWave(double a)
{
    double reflected1 = a - sub_res[R1Div]
        * (cmp1.ref_wave + cmp2.ref_wave);
    double reflected2 = a + sub_res[R2Div]
        * (cmp1.ref_wave - cmp2.ref_wave);
    cmp1.receiveIncidentWave(reflected1);
    cmp2.receiveIncidentWave(reflected2);
    inc_wave = a;
}

template <class CMP_TP_1, class CMP_TP_2>
double ParallelThreePort<CMP_TP_1, CMP_TP_2>::emitReflectedWave()
{
    ref_wave = sub_res[R2Div] * cmp1.emitReflectedWave()
        + sub_res[R1Div] * cmp2.emitReflectedWave();
    return ref_wave;
}

template <class CMP_TP_1, class CMP_TP_2>
void ParallelThreePort<CMP_TP_1, CMP_TP_2>::calcPortResistance()
{
    port_res = sub_res[R1] * sub_res[R2] / sub_res[RSer];
}


#endif /* va_ports_impl_h */
