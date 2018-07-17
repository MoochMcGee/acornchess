#include "common.h"
#include "i808x_cpu.h"
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
        i808x_cpu cpu;
        cpu.type = i808x_cpu_type::i8088;

        cpu.init();

        dev.init();

        cpu.device = &dev;

        cpu.rb_real = ibm5150_rb;
        cpu.rw_real = ibm5150_rw;
        cpu.wb_real = ibm5150_wb;
        cpu.ww_real = ibm5150_ww;

        cpu.iorb_real = ibm5150_iorb;
        cpu.iorw_real = ibm5150_iorw;
        cpu.iowb_real = ibm5150_iowb;
        cpu.ioww_real = ibm5150_ioww;

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

        for(int i = 0; i < 10; i++)
        {
            cpu.tick();
        }

        dev.exit();
    }

    return 0;
}
