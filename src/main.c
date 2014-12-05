#include "application.h"

#include "config.h"

int main(int argc, char* argv[]) {
    config_init();
    config_load("test.ini");
    Ini* test = config_get("test.ini");

    return app_run(argc, argv);
}
