#include "include/compile.h"

int main(int argc, const char *argv[]){
    
    char *directory = "../testcases/";
    char path[256];
    
    if(argv[argc-1] == NULL){
        printf("No file specified");
        return 0;
    }
    
    snprintf(path, sizeof(path), "%s%s", directory, argv[argc-1]);

    compile(path);
    
    return 0;
}
