//#include "application.h"
#include <mxml.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* fp;
    mxml_node_t* tree;
    fp = fopen("test.xml", "rb");
    tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);
    fclose(fp);
    return 0;
    //return app_run(argc, argv);
}
