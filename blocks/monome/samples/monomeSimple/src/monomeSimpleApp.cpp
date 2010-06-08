#include "cinder/app/AppBasic.h"
#include "OscMonome64.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class monomeSimpleApp : public AppBasic {
  public:
    void prepareSettings(Settings *settings);
    void setup();
    void draw();

    void onMonomeButton(MonomeButtonEvent* e);

    OscMonome64 monome;
};

void monomeSimpleApp::prepareSettings(Settings *settings)
{
    settings->setFrameRate(30.0f);
    settings->setWindowSize(512, 512);
}

void monomeSimpleApp::setup()
{
    monome.init("localhost", 8000, 8080);
    monome.addCallback(boost::bind(&monomeSimpleApp::onMonomeButton, this, _1));
    monome.clearLeds(OscMonome64::OFF);
}

void monomeSimpleApp::onMonomeButton(MonomeButtonEvent* e)
{
	if(e->state == OscMonome64::DOWN){
    	monome.toggleLed(e->x, e->y);
    }
}

void monomeSimpleApp::draw()
{
    gl::clear();

    float lw = (float)getWindowWidth() / monome.nx;
    float lh = (float)getWindowHeight() / monome.ny;
    for(int x = 0; x < monome.nx; x++){
        for(int y = 0; y < monome.ny; y++){
        	if(monome.getLed(x, y) == OscMonome64::ON){
                glColor3f(1, 1, 1);
                gl::drawSolidRect(Rectf(
                	lw * x + 10,      lh * y + 10,
                    lw * x + lw - 10, lh * y + lh - 10
            	));
            }
        }
    }
}

// This line tells Cinder to actually create the application
CINDER_APP_BASIC( monomeSimpleApp, RendererGl )
