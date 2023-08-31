//
//  va_ports.h
//  ViraFilter
//
//  Created by Lucas Scheidt on 31.08.23.
//  Copyright © 2023 LeadBackToSines. All rights reserved.
//

#ifndef va_ports_h
#define va_ports_h

#include "va_variables.h"

enum Resistances
{
    R1, R2, RSer, R1Div, R2Div, sub_res_size
};

template <class CMP_TP_1, class CMP_TP_2>
class SerialThreePort
{
public:
    SerialThreePort() = delete;
    SerialThreePort(CMP_TP_1& _cmp1, CMP_TP_2& _cmp2);
    void updatePort();
    void receiveIncidentWave(double a);
    double emitReflectedWave();
    void calcPortResistance();
    ~SerialThreePort() {}
    VA_VARIABLES
private:
    double sub_res[sub_res_size];
    CMP_TP_1& cmp1;
    CMP_TP_2& cmp2;
};

template <class CMP_TP_1, class CMP_TP_2>
class ParallelThreePort
{
public:
    ParallelThreePort() = delete;
    ParallelThreePort(CMP_TP_1& _cmp1, CMP_TP_2& _cmp2);
    void updatePort();
    void receiveIncidentWave(double a);
    double emitReflectedWave();
    void calcPortResistance();
    ~ParallelThreePort() {}
    VA_VARIABLES
private:
    double sub_res[sub_res_size];
    CMP_TP_1& cmp1;
    CMP_TP_2& cmp2;
};

#endif /* va_ports_h */
