#include "i808x_cpu.h"

void i808x_cpu::init()
{
    for(int i = 0; i < 8; i++)
    {
        r[i].w = 0;
    }

    cs = 0xffff;
    ds = es = ss = 0;
    ip = 0;
    flags.whole = 0xf002;
}

u8 i808x_cpu::rb(u16 seg, u16 offset)
{
    return rb_real(device, (((u32)seg << 4) + offset) & 0xfffff);
}

u16 i808x_cpu::rw(u16 seg, u16 offset)
{
    return rw_real(device, (((u32)seg << 4) + offset) & 0xfffff);
}

void i808x_cpu::wb(u16 seg, u16 offset, u8 data)
{
    wb_real(device, ((u32)seg << 4) + offset, data);
}

void i808x_cpu::ww(u16 seg, u16 offset, u16 data)
{
    ww_real(device, ((u32)seg << 4) + offset, data);
}

u8 i808x_cpu::iorb(u16 addr)
{
    return iorb_real(device, addr);
}

u16 i808x_cpu::iorw(u16 addr)
{
    return iorw_real(device, addr);
}

void i808x_cpu::iowb(u16 addr, u8 data)
{
    iowb_real(device, addr, data);
}

void i808x_cpu::ioww(u16 addr, u16 data)
{
    ioww_real(device, addr, data);
}

void i808x_cpu::tick()
{
    //TODO
    u8 opcode = rb(cs, ip);
    printf("Opcode:%08x\nCS:%04x\nIP:%04x\n", opcode, cs, ip);
    printf("DS:%04x\nES:%04x\nSS:%04x\n", ds, es, ss);
    printf("AX:%04x\nBX:%04x\nCX:%04x\nDX:%04x\nSP:%04x\nBP:%04x\nSI:%04x\nDI:%04x\n", r[0], r[3], r[1], r[2], r[4], r[5], r[6], r[7]);
}

void i808x_cpu::run(int insns)
{
    for(int i = 0; i<insns; i++)
    {
        tick();
    }
}