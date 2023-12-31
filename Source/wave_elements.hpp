//
//  wave_elements.hpp
//  va_dev_2
//
//  Created by Lucas Scheidt on 21.04.23.
//

//#ifndef wave_elements_hpp
//#define wave_elements_hpp
//
//enum Resistances
//{
//    R1, R2, RSer, R1Div, R2Div, sub_res_size
//};
//
//class WaveElement
//{
//public:
//    virtual void receiveIncidentWave(double a) = 0;
//    virtual double emitReflectedWave() = 0;
//    virtual const double* getPortResistance() const = 0;
//    virtual void calcPortResistance() = 0;
//    virtual ~WaveElement() {}
//    double ref_wave = 0.0;
//    double inc_wave = 0.0;
//    double port_res = 1.0;
//};
//
//class NodeWaveElement : public WaveElement
//{
//private:
//    const double* connected_res[2];
//protected:
//    // Because the algorithm has to be fast,
//    // the array is talked to directly by subclasses.
//    // Same goes for the waves, because they are updated every sample.
//    // The port resistance can change too when a parameter is changed.
//    double sub_res[sub_res_size];
//    WaveElement* cmp1;
//    WaveElement* cmp2;
//public:
//    NodeWaveElement(WaveElement* _cmp1, WaveElement* _cmp2);
//    void updatePort();
//    void setConnections(WaveElement* _cmp1, WaveElement* _cmp2);
//    const double* getPortResistance() const override { return &port_res; }
//    ~NodeWaveElement() {}
//};
//
//class SerialThreePort : public NodeWaveElement
//{
//public:
//    SerialThreePort() : NodeWaveElement{nullptr, nullptr} {}
//    SerialThreePort(WaveElement* _cmp1, WaveElement* _cmp2);
//    void receiveIncidentWave(double a) override;
//    double emitReflectedWave() override;
//    void calcPortResistance() override;
//    ~SerialThreePort() {}
//};
//
//class ParallelThreePort : public NodeWaveElement
//{
//public:
//    ParallelThreePort() : NodeWaveElement{nullptr, nullptr} {}
//    ParallelThreePort(WaveElement* _cmp1, WaveElement* _cmp2);
//    void receiveIncidentWave(double a) override;
//    double emitReflectedWave() override;
//    void calcPortResistance() override;
//    ~ParallelThreePort() {}
//};
//
//class LeafWaveElement : public WaveElement
//{
//protected:
//    // These ones are called internally only and should be updated fast.
//    double cmp_val;
//    double samplerate = 44100;
//public:
//    void updateComponent(double& _cmp_val);
//    void updateComponent(double&& _cmp_val);
//    void receiveIncidentWave(double a) override { inc_wave = a; }
//    void setSampleRate(double _sr) { samplerate = _sr; }
//    double getSampleRate() const { return samplerate; }
//    const double* getPortResistance() const override { return &port_res; };
//    virtual ~LeafWaveElement() {};
//};
//
//class Resistor : public LeafWaveElement
//{
//public:
//    Resistor() { cmp_val = 1e3; }
//    Resistor(double _cmp_val) { updateComponent(_cmp_val); }
//    double emitReflectedWave() override { return 0.0; }
//    void calcPortResistance() override { port_res = cmp_val; }
//    ~Resistor() {}
//};
//
//class Inductor : public LeafWaveElement
//{
//public:
//    Inductor() { cmp_val = 1e-3; }
//    Inductor(double _cmp_val) { updateComponent(_cmp_val); }
//    double emitReflectedWave() override;
//    void calcPortResistance() override;
//    ~Inductor() {}
//};
//
//class Capacitor : public LeafWaveElement
//{
//public:
//    Capacitor() { cmp_val = 10e-6; }
//    Capacitor(double _cmp_val) { updateComponent(_cmp_val); }
//    double emitReflectedWave() override;
//    void calcPortResistance() override;
//    ~Capacitor() {}
//};
//
//class ResistiveVS : public LeafWaveElement
//{
//public:
//    // By default the port Resistance is set to 1
//    // When used as root element this leads to no changes in sound
//    // and has no effect to any other objects.
//    ResistiveVS() : cmp{nullptr} { port_res = 1.0; }
//    void processSample(double _sample) { v_in = _sample; }
//    double emitReflectedWave() override { return v_in * port_res; }
//    
//    // Only needed if NOT the root element.
//    void setConnection(WaveElement* _cmp);
//    void calcPortResistance() override { port_res = cmp->port_res; }
//    
//    ~ResistiveVS() {}
//private:
//    WaveElement* cmp;
//    double v_in;
//};
//
//class IdealVS : public LeafWaveElement
//{
//public:
//    IdealVS(){}
//    void processSample(double _sample) { v_in = _sample; }
//    double emitReflectedWave() override { return 2 * v_in - inc_wave; }
//    void calcPortResistance() override {}
//private:
//    double v_in;
//};
//
//double inline virtualVoltage(WaveElement* cmp)
//{
//    return (cmp->inc_wave + cmp->ref_wave) * 0.5;
//}
//
//#endif /* wave_elements_hpp */
