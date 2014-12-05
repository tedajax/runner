#include "application.h"

#include "ini.h"

int main(int argc, char* argv[]) {
    Ini test;
    ini_load(&test, "test.ini");

    char* a = ini_get_string(&test, NULL, "globalkey");
    bool b = ini_get_bool(&test, "test", "key6");

    return app_run(argc, argv);
}
