#ifndef RALLYCONF_H
#define RALLYCONF_H

#include <Preferences.h>

class RallyConf {
public:
    void begin(void);

private:
    Preferences preferences;
};

#endif