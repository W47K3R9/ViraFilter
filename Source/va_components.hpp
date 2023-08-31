//
//  va_components.hpp
//  ViraFilter
//
//  Created by Lucas Scheidt on 31.08.23.
//  Copyright © 2023 LeadBackToSines. All rights reserved.
//

#ifndef va_components_hpp
#define va_components_hpp

#include "va_variables.h"

class LeafWaveElement
{
protected:
    // These ones are called internally only and should be updated fast.
    double cmp_val;
    double samplerate = 44100;
public:
    void receiveIncidentWave(double a) { inc_wave = a; }
    void setSampleRate(double _sr) { samplerate = _sr; }
    double getSampleRate() const { return samplerate; }
    const double getPortResistance() const { return port_res; };
    virtual ~LeafWaveElement() {};
    VA_VARIABLES
};

class Resistor : public LeafWaveElement
{
public:
    Resistor() { cmp_val = 1e3; }
    Resistor(double _cmp_val) { calcPortResistance(_cmp_val); }
    double emitReflectedWave() { return 0.0; }
    void calcPortResistance(double& _cmp_val);
    void calcPortResistance(double&& _cmp_val);
    ~Resistor() {}
};

class Inductor : public LeafWaveElement
{
public:
    Inductor() { cmp_val = 1e-3; }
    Inductor(double _cmp_val) { calcPortResistance(_cmp_val); }
    double emitReflectedWave();
    void calcPortResistance(double& _cmp_val);
    void calcPortResistance(double&& _cmp_val);
    ~Inductor() {}
};

class Capacitor : public LeafWaveElement
{
public:
    Capacitor() { cmp_val = 10e-6; }
    Capacitor(double _cmp_val) { calcPortResistance(_cmp_val); }
    double emitReflectedWave();
    void calcPortResistance(double& _cmp_val);
    void calcPortResistance(double&& _cmp_val);
    ~Capacitor() {}
};

template <class CMP_TP>
class ResistiveVS : public LeafWaveElement
{
public:
    // By default the port Resistance is set to 1
    // When used as root element this leads to no changes in sound
    // and has no effect to any other objects.
    ResistiveVS() : cmp{nullptr} { port_res = 1.0; }
    void processSample(double _sample) { v_in = _sample; }
    double emitReflectedWave() { return v_in * port_res; }
    
    // Only needed if NOT the root element.
    void setConnection(CMP_TP& _cmp)
        { cmp = _cmp; calcPortResistance(); };
    void calcPortResistance() { port_res = cmp.port_res; }
    
    ~ResistiveVS() {}
private:
    CMP_TP& cmp;
    double v_in;
};

class IdealVS : public LeafWaveElement
{
public:
    IdealVS(){}
    void processSample(double _sample) { v_in = _sample; }
    double emitReflectedWave() { return 2 * v_in - inc_wave; }
private:
    double v_in;
};

template <class CMP_TP>
double inline virtualVoltage(CMP_TP& cmp)
{
    return (cmp.inc_wave + cmp.ref_wave) * 0.5;
}


#endif /* va_components_hpp */
