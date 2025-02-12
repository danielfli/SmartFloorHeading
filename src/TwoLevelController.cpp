#include "TwoLevelController.hpp"

namespace sfh
{

TwoLevelController::TwoLevelController(const u_int hysteresisPercent)
    : _setvalue_w(0), _actualvalue_x(10.0), _e(0), _w_upperLimit(0), _w_lowerLimit(0),
      _hysteresis(static_cast<double>(hysteresisPercent)), _y_output(0)
{
    if (hysteresisPercent > 100)
        _hysteresis = 100;

    _delta = (_hysteresis / 100.0);

    //set controller state to positve controlling
    _state = 10;
}

TwoLevelController::~TwoLevelController()
{
}

int TwoLevelController::GetControllerOutput_Y()
{
    return _y_output;
}

void TwoLevelController::SetSetpoint_W(double SetValue, double ActualValue)
{
    _actualvalue_x = ActualValue;

    if (SetValue < 0)
    {
        std::cerr << "Error - negativ SetValue not permitted..\n";
        // take old value
    }
    else
    {
        _setvalue_w = SetValue;
    }

    // calculation

    _w_lowerLimit = _setvalue_w - (_setvalue_w * _delta);
    _w_upperLimit = _setvalue_w + (_setvalue_w * _delta);

    // differenz set output
    _e = _setvalue_w - _actualvalue_x;

    switch (_state)
    {
    case 10:
        // to point A - upper limit
        _y_output = 1;

        if (_actualvalue_x >= _w_upperLimit)
        {
            _y_output = -1; // off
            _state = 20;
        }
        break;

    case 20:
        //to point B - lower limit
        _y_output = -1; // off
        if (_actualvalue_x <= _w_lowerLimit)
        {
            _y_output = 1; // on
            _state = 10;
        }
        break;

    default:
        break;
    }

#ifdef DEBUG
    std::cout << "______________________________________________________________________________________________\n";
    std::cout << "controller value:\n";
    std::cout << "Set : " << _setvalue_w << " - upLimit : " << _w_upperLimit << " - lowLimit : " << _w_lowerLimit
              << "  - Actual: " << _actualvalue_x;
    std::cout << " - delta: " << _delta;
    std::cout << " - controller e: " << _e;
    std::cout << " - Output set: " << _y_output;
    std::cout << "\n______________________________________________________________________________________________\n";
#endif
}
} // namespace sfh