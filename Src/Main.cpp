///
////////////////////////////////////////////////////////////////////////////////

#include "Sandbox.h"



int main(int argc, char** argv)
{
    Sandbox* sandbox = new Sandbox();
    sandbox->Run();

    delete sandbox;
    
    return 0;
}