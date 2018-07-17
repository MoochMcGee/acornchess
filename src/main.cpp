#include "common.h"
#include "ibm5150.h"

enum class pc_model
{
    ibm5150
};

int main(int ac, char** av)
{
    if(ac < 2)
    {
        printf("usage: %s [model]\n", av[0]);
        printf("model can be \"ibm5150\". No other models are supported at this time.\n");
        return 1;
    }

    std::string model = av[1];
    pc_model pc_type;

    if(model == "ibm5150")
    {
        pc_type = pc_model::ibm5150;

        ibm5150 dev;

        dev.init();

        FILE* fp = fopen("roms/machines/ibmpc/pc102782.bin","rb");
        if(!fp)
        {
            printf("unable to open roms/machines/ibmpc/pc102782.bin, are you sure it exists?\n");
            return 3;
        }
        if(fread(dev.bios, 1, 0x2000, fp) != 0x2000)
        {
            fclose(fp);
            return 4;
        }
        fclose(fp);

        /*for(int i = 0; i < 500; i++)
        {
        }*/

        printf("Opcode:%02x\n", ibm5150_rb(&dev, 0xffff0));

        dev.exit();
    }

    return 0;
}
