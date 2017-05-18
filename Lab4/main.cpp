#include "project.h"

int main(int argc, char *argv[])
{
    if(argc > 2)                    //starting with arguments
    {
        int arg1, arg2;
        sscanf(argv[1],"%d",&arg1);
        sscanf(argv[2],"%d",&arg2);

        Project project4(arg1,arg2);
        project4.START();
    }
    else                            //starting by default
    {
        Project project4;
        project4.START();
    }

    return 0;
}

