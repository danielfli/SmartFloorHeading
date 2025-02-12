#pragma once
#include "../include/smartfloorheating/HeatingConstruction.hpp"
#include <iostream>
#include <vector>

namespace sfh
{

class Output
{
  private:
    bool InitSuccess;
    bool _verbose;
    std::vector<DeviceHeaterID> _output;
    static int instance; //ToDo: sigelton create

  public:
    Output(std::vector<DeviceHeaterID> &vecdeviceoutput, const size_t num, bool verbose = false);
    ~Output();

    operator bool()
    {
        return InitSuccess;
    }

    void SwitchAllOn() const;

    void SwitchAllOff() const;

    void TurnOn(const size_t pinId) const;
    void TurnOff(const size_t pinId) const;

    std::vector<DeviceHeaterID> GetSwitches() const;
};

} // namespace sfh