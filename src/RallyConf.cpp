#include "RallyConf.h"


static const char* BUTTON1_CONF = "button1";

void RallyConf::begin()
{
    preferences.begin(BUTTON1_CONF, false);

    bool tpInit = preferences.isKey("nvsInit");  
    if (!tpInit) {
        loadDefaultPref();
    }

    preferences.end();


    

}

void RallyConf::loadDefaultPref() {

}

