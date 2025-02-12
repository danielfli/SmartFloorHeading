#include "Operational.hpp"
#include <chrono>
#include <iostream>
#include <thread>

namespace sfh
{

Operational::Operational(Thermostat &thermostat, const Output &output, const u_int percent)
    : _thermostat(thermostat), _output(output)
{
    Thermostat_Data thermostatData{};
    auto distributorSize = _output.GetSwitches().size();
    auto sensorSize = thermostat.GetInputDevices().size();
    //checkup sensor equals distributor
    if (distributorSize != sensorSize)
    {
        std::cout << "WARING! Different configuration using size of distributor:" << distributorSize << "\n";
    }

    //Get Thermostat data
    const std::vector<sfh::DeviceThermostat> thermostats = _thermostat.GetInputDevices();

    //Setup two-level-controller
    for (size_t i = 0; i < sensorSize; i++)
    {
        TwoLevelController controller{percent};
        _veccontrol.push_back(controller);
    }

    std::cout << "_____SETUP CONTROLLER_____\n";
    std::cout << "controller: " << _veccontrol.size() << "\n";
}

Operational::~Operational()
{
}

bool Operational::Run()
{
    std::cout << "\n\n_____START CONTROLLING HEATING_____\n";
    std::vector<sfh::DeviceHeaterID> switches = _output.GetSwitches();
    std::vector<sfh::DeviceThermostat> thermostats = _thermostat.GetInputDevices();
    auto distributor = _thermostat.GetOutputDevices();

    //! 1. Input 0 - x einlesen
    //! 2. Verarbeiten 0 - x verarbeiten
    //! 3. Output  (3.1: State , 3.2: schalten )  0 - x ausgeben
    size_t round_counter = 0;
    Thermostat_Data thermostatData{};
    Heater_Data distributorStatePre{};
    Heater_Data distributorStatePost{};
    int controllerOutput_X = 0;
    while (true)
    {
#ifdef DEBUG
        std::cout << "\n\ncylce round: " << ++round_counter << "\n";
        std::cout << "\n\n";
#endif
        std::cout << "-----------------------------------\n";

        for (size_t i = 0; i < distributor.size(); i++)
        {
            //!input
            distributorStatePre = _thermostat.GetStateData(distributor[i].entity_id, false);
            thermostatData = _thermostat.GetTherostatData(thermostats[i].id, false);

            //!work
            switch (thermostatData.state)
            {
            case States::off:
                std::cout << "Room: " << thermostatData.friendly_name
                          << " - Current Temp: " << thermostatData.currentValueTemp << " °C HEATING OFF"
                          << "\n";
                _output.TurnOff(switches[i].id);
                distributorStatePost = _thermostat.SetHeaterState({false, distributor[i].entity_id}, false);
                break;

            case States::idle:
                std::cout << "Room: " << thermostatData.friendly_name
                          << " - Current Temp: " << thermostatData.currentValueTemp << " °C HEATING IDLE"
                          << "\n";
                _veccontrol[i].SetSetpoint_W(thermostatData.setValueTemp, thermostatData.currentValueTemp);

                //!output
                controllerOutput_X = _veccontrol[i].GetControllerOutput_Y();
                if (controllerOutput_X == 1)
                {
                    _output.TurnOn(switches[i].id);
                    distributorStatePost = _thermostat.SetHeaterState({true, distributor[i].entity_id}, false);
                }
                else if (controllerOutput_X == -1)
                {
                    _output.TurnOff(switches[i].id);
                    distributorStatePost = _thermostat.SetHeaterState({false, distributor[i].entity_id}, false);
                }
                break;

            case States::heating:
                std::cout << "Room: " << thermostatData.friendly_name
                          << " - Current Temp: " << thermostatData.currentValueTemp << " °C HEATING ON"
                          << "\n";
                _veccontrol[i].SetSetpoint_W(thermostatData.setValueTemp, thermostatData.currentValueTemp);

                //!output
                controllerOutput_X = _veccontrol[i].GetControllerOutput_Y();
                if (controllerOutput_X == 1)
                {
                    _output.TurnOn(switches[i].id);
                    distributorStatePost = _thermostat.SetHeaterState({true, distributor[i].entity_id}, false);
                }
                else if (controllerOutput_X == -1)
                {
                    _output.TurnOff(switches[i].id);
                    distributorStatePost = _thermostat.SetHeaterState({false, distributor[i].entity_id}, false);
                }
                break;

            default:
                break;
            }

            //!Log
#ifdef DEBUG
            std::cout << std::boolalpha << "Distributor States - pre: " << distributorStatePre.state
                      << " - post: " << distributorStatePost.state << "\n\n";
#endif
                    }

        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return true;
}

} // namespace sfh