#include "ibm5150.h"

void ibm5150::init()
{
    bios = (u8*)calloc(0x2000, 1);
    ram = (u8*)calloc(0x40000, 1);

    reg_access_log = fopen("reglog.txt","w+");
}

void ibm5150::exit()
{
    free(bios);
    free(ram);
    if(reg_access_log) fclose(reg_access_log);
}

u8 ibm5150_rb(void* dev, u64 addr)
{
    ibm5150* device = (ibm5150*) dev;
    if(addr < 0x4000)
    {
        return device->ram[addr & 0x3fff];
    }
    else if(addr >= 0xfe000 && addr < 0x100000)
    {
        return device->bios[addr & 0x1fff];
    }
    return 0;
}

u16 ibm5150_rw(void* dev, u64 addr)
{
    ibm5150* device = (ibm5150*) dev;
    if(addr & 1)
    {
        return ibm5150_rb(dev, addr) | ((u16)ibm5150_rb(dev, addr + 1) << 8);
    }
    else
    {
        if(addr < 0x4000)
        {
            return *(u16*)(device->ram[addr & 0x3ffe]);
        }
        else if(addr >= 0xfe000 && addr < 0x100000)
        {
            return *(u16*)(device->bios[addr & 0x1ffe]);
        }
    }
    return 0;
}

void ibm5150_wb(void* dev, u64 addr, u8 data)
{
    ibm5150* device = (ibm5150*) dev;
    if(addr < 0x4000)
    {
        device->ram[addr & 0x3fff] = data;
    }
}

void ibm5150_ww(void* dev, u64 addr, u16 data)
{
    ibm5150* device = (ibm5150*) dev;
    if(addr & 1)
    {
        ibm5150_wb(dev, addr, data & 0xff);
        ibm5150_wb(dev, addr + 1, data >> 8);
    }
    else
    {
        if(addr < 0x4000)
        {
            *(u16*)(device->ram[addr & 0x3ffe]) = data;
        }
    }
}

u8 ibm5150_iorb(void* dev, u16 addr)
{
    ibm5150* device = (ibm5150*) dev;
    fprintf(device->reg_access_log, "Unknown I/O port access at addr %04x\n", addr);
    return 0;
}

u16 ibm5150_iorw(void* dev, u16 addr)
{
    ibm5150* device = (ibm5150*) dev;
    if(addr & 1)
    {
        return ibm5150_iorb(dev, addr) | ((u16)ibm5150_iorb(dev, addr + 1) << 8);
    }
    else
    {
        return 0;
    }
    return 0;
}

void ibm5150_iowb(void* dev, u16 addr, u8 data)
{
    ibm5150* device = (ibm5150*) dev;
    fprintf(device->reg_access_log, "Unknown I/O port access at addr %04x data %08x\n", addr, data);
}

void ibm5150_ioww(void* dev, u16 addr, u16 data)
{
    ibm5150* device = (ibm5150*) dev;
    if(addr & 1)
    {
        ibm5150_iowb(dev, addr, data & 0xff);
        ibm5150_iowb(dev, addr + 1, data >> 8);
    }
    else
    {
        return;
    }
}