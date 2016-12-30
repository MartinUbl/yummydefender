#include "general.h"
#include "Application.h"

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

int main(int argc, char** argv)
{
    if (!sApplication->Init())
        return 1;

    return sApplication->Run();
}
