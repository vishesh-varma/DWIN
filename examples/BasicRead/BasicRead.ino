#include <DWIN.h>

void setup()
{
    DWIN.begin(115200);
}

void loop()
{
    if(DWIN.read())
    {
        DWINdata = DWIN.getDisplayData();

        switch(DWINdata.command)
        {
            case WRITE_COMMAND:
                switch (DWINdata.address)
                {
                case 0:
                    break;
                
                default:
                    break;
                }
        }
    }
}