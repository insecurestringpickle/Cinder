#pragma once

#include "cinder/Cinder.h"
#include "OscSender.h"
#include "OscListener.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <math.h>
#include <iostream.h>

#include <list>

using namespace std;

namespace cinder{ namespace monome{

struct MonomeButtonEvent
{
    int x, y, state;
};

class Monome64
{
  public:
    Monome64();
    ~Monome64();

    void init(string _host, int _port_in, int _port_out);
    void setBaseName(string _basename);

    void addCallback(boost::function<void(MonomeButtonEvent*)> callback);
    void onOscMessage(osc::Message* msg);

    void setLed(int x, int y, int state);
    void setLedCol(int x, int states[]);
    void setLedRow(int y, int states[]);

    void toggleLed(int x, int y);
    void toggleLedCol(int x);
    void toggleLedRow(int y);

    int getLed(int x, int y);

    void clearLeds(int state);

    static const int ON, OFF, UP, DOWN;

    osc::Sender   osc_out;
    osc::Listener osc_in;

    string osc_host;
    int    osc_port_in;
    int    osc_port_out;

    string basename;
    string press_addr, led_addr, led_col_addr, led_row_addr, led_frame_addr, led_clear_addr, led_intensity_addr;

    static const int nx = 8, ny = 8; // HACK: this should be variable

    int ledState[nx][ny];
    int buttonState[nx][ny];


  protected:
    void setLedColRow(string addr, int i, int states[], int length);
    void msgPackStates(osc::Message* msg, int states[], int length);

    list< boost::function<void(MonomeButtonEvent*)> > callbacks;
};

}}
