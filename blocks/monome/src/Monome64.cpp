#include "Monome64.h"

namespace cinder{ namespace monome{


const int Monome64::ON  = 1;
const int Monome64::OFF = 0;

const int Monome64::UP   = 0;
const int Monome64::DOWN = 1;


Monome64::Monome64()
{
    setBaseName("/64");
}
Monome64::~Monome64()
{
}


void Monome64::init(string _host, int _port_in, int _port_out)
{
    osc_host     = _host;
    osc_port_in  = _port_in;
    osc_port_out = _port_out;

    osc_in.setup(osc_port_in);
    osc_in.addCallback(boost::bind(&Monome64::onOscMessage, this, _1));

    osc_out.setup(osc_host, osc_port_out);
}

void Monome64::setBaseName(string _basename)
{
    basename = _basename[0] == '/' ? _basename : "/" + _basename;

    press_addr         = basename + "/press";

    led_addr           = basename + "/led";
    led_col_addr       = basename + "/led_col";
    led_row_addr       = basename + "/led_row";
    led_frame_addr     = basename + "/frame";
    led_clear_addr     = basename + "/clear";
    led_intensity_addr = basename + "/intensity";
}


void Monome64::addCallback(boost::function<void(MonomeButtonEvent*)> callback)
{
    callbacks.push_back(callback);
}

void Monome64::onOscMessage(osc::Message* msg)
{
	if(msg->getAddress() == press_addr){
        MonomeButtonEvent event;

        event.x     = msg->getArgAsInt32(0, false);
        event.y     = msg->getArgAsInt32(1, false);
        event.state = msg->getArgAsInt32(2, false);

        // Dispatch Event
        list< boost::function<void(MonomeButtonEvent*)> >::iterator callback = callbacks.begin();
        for(; callback != callbacks.end(); callback++){
            if(!callback->empty())
                (*callback)(&event);
        }
    }
}


int Monome64::getLed(int x, int y)
{
    return ledState[y][x];
}

void Monome64::setLed(int x, int y, int state)
{
    osc::Message msg;
    msg.setAddress(led_addr);
    msg.setRemoteEndpoint(osc_host, osc_port_in);

    msg.addIntArg(x);
    msg.addIntArg(y);
    msg.addIntArg(state);

    osc_out.sendMessage(msg);

    ledState[y][x] = state;
}
void Monome64::toggleLed(int x, int y)
{
    setLed(x, y, ledState[y][x] == OFF ? ON : OFF);
}


void Monome64::setLedCol(int x, int states[])
{
    setLedColRow(led_col_addr, x, states, ny);

    for(int i = ny; --i >= 0;)
        ledState[i][x] = states[i];
}
void Monome64::toggleLedCol(int x)
{
    int col[nx];
    for(int i = ny; --i >= 0;)
        col[i] = ledState[i][x] == OFF ? ON : OFF;
    setLedCol(x, col);
}

void Monome64::setLedRow(int y, int states[])
{
    setLedColRow(led_row_addr, y, states, nx);

    for(int i = nx; --i >= 0;)
        ledState[y][i] = states[i];
}
void Monome64::toggleLedRow(int y)
{
    int row[ny];
    for(int i = nx; --i >= 0;)
        row[i] = ledState[y][i] == OFF ? ON : OFF;
    setLedRow(y, row);
}

void Monome64::setLedColRow(string addr, int i, int states[], int length)
{
    osc::Message msg;
    msg.setAddress(addr);
    msg.setRemoteEndpoint(osc_host, osc_port_in);

    msg.addIntArg(i);
    msgPackStates(&msg, states, length);

    osc_out.sendMessage(msg);
}


void Monome64::clearLeds(int state)
{
    osc::Message msg;
    msg.setAddress(led_clear_addr);
    msg.setRemoteEndpoint(osc_host, osc_port_in);

    msg.addIntArg(state);

    osc_out.sendMessage(msg);

    for(int y = ny; --y >= 0;)
        for(int x = nx; --x >= 0;)
            ledState[y][x] = state;
}


void Monome64::msgPackStates(osc::Message* msg, int states[], int length)
{
    for(int s = 0, i = 0, n = (int)ceil(length / 32.0); i < n; i++){
        int32_t packed = 0;
        for(int b = 0; b < 32 && s < length; b++){
            packed |= states[s++] << b;
        }
        msg->addIntArg(packed);
    }
}


}}
