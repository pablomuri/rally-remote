#include "RallyConf.h"


static const char* BUTTON1_CONF = "button1";

void RallyConf::begin(void)
{
    preferences.begin(BUTTON1_CONF, false);
}

