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

typedef boost::function<void(MonomeButtonEvent*)> MonomeCallback;

class Monome
{
  public:
    Monome();
    Monome(string _basename, int _nx, int _ny);
    ~Monome();

    void init(string _host, int _port_in, int _port_out);
    void setBaseName(string _basename);
    void setSize(int _nx, int _ny);

    void addCallback(MonomeCallback callback);
    void onOscMessage(const osc::Message* msg);

    void setLed(int x, int y, int state);
    void setLedCol(int x, int states[]);
    void setLedRow(int y, int states[]);

    void toggleLed(int x, int y);
    void toggleLedCol(int x);
    void toggleLedRow(int y);

    void setLedFrame(int states[]);

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

    int nx, ny;
    int *ledState, *buttonState;


  protected:
    void wipeArrays();

    void setLedColRow(string addr, int i, int states[], int length);
    void msgPackStates(osc::Message* msg, int states[], int length);

    list<MonomeCallback> callbacks;
};

class Monome64 : public Monome
{
  public:
	Monome64() : Monome("/64", 8, 8){}
};

class Monome128 : public Monome
{
  public:
    Monome128() : Monome("/128", 16, 8){}
};

class Monome256 : public Monome
{
  public:
    Monome256() : Monome("/256", 16, 16){}
};

}}
