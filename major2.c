#include "major2.h"

int main(int argc, char *argsv[])
{
        if(argc > 2)
                exit(0);

        int errorCheck;
        if(argsv[1] == NULL)
                errorCheck = shell();
        else
                errorCheck = batchProcesser(argsv[1]);

        return errorCheck;
}
