#pragma once

#include "common.h"

enum class i808x_cpu_type
{
    i8088, i8086, nec_v20, nec_v30
};

struct i808x_cpu
{
    enum
    {
        ax = 0, cx, dx, bx, sp, bp, si, di,
        es = 0, cs, ss, ds
    };
    union
    {
        struct
        {
#ifdef LITTLE_ENDIAN
            u8 l;
            u8 h;
#else
            u8 h;
            u8 l;
#endif
        };
        u16 w;
    } r[8];

    u16 sr[4];

    u16 ip;

    union
    {
        struct
        {
#ifdef LITTLE_ENDIAN
            u16 carry : 1;
            u16 reserved1 : 1;
            u16 parity : 1;
            u16 reserved2 : 1;
            u16 adjust : 1;
            u16 reserved3 : 1;
            u16 zero : 1;
            u16 sign : 1;

            u16 trap : 1;
            u16 interrupt_enable : 1;
            u16 direction : 1;
            u16 overflow : 1;
            u16 reserved4 : 4;
#else
            u16 trap : 1;
            u16 interrupt_enable : 1;
            u16 direction : 1;
            u16 overflow : 1;
            u16 reserved4 : 4;

            u16 carry : 1;
            u16 reserved1 : 1;
            u16 parity : 1;
            u16 reserved2 : 1;
            u16 adjust : 1;
            u16 reserved3 : 1;
            u16 zero : 1;
            u16 sign : 1;
#endif
        };
        u16 whole;
    } flags;

    int segment_override; //defaults to -1 for no segment override.

    i808x_cpu_type type;
    void* device;

    std::function<u8(void*,u64)> rb_real;
    std::function<u16(void*,u64)> rw_real;
    std::function<void(void*,u64,u8)> wb_real;
    std::function<void(void*,u64,u16)> ww_real;

    std::function<u8(void*,u16)> iorb_real;
    std::function<u16(void*,u16)> iorw_real;
    std::function<void(void*,u16,u8)> iowb_real;
    std::function<void(void*,u16,u16)> ioww_real;

    void init();

    u8 rb(u16 seg, u16 offset);
    u16 rw(u16 seg, u16 offset);
    void wb(u16 seg, u16 offset, u8 data);
    void ww(u16 seg, u16 offset, u16 data);
    u8 iorb(u16 addr);
    u16 iorw(u16 addr);
    void iowb(u16 addr, u8 data);
    void ioww(u16 addr, u16 data);

    u64 get_ea(u8 modrm);
    u8 get_rm_byte(u8 modrm);
    void tick();
    void run(int insns);
};