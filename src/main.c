#include "application.h"
#include "ini.h"

int main(int argc, char* argv[]) {
    Ini test;
    ini_load(&test, "test.ini");
    char* s = ini_get_string(&test, "section", "key2");
    int i = ini_get_int(&test, "section", "key1");
    printf("%s %d\n", s, i);
    ini_free(&test);
    
    //return app_run(argc, argv);
    getc(stdin);
    return 0;
}
