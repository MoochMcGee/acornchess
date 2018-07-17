#pragma once

#include "common.h"

struct ibm5150
{
    u8* bios; //[0x2000]
    u8* ram; //[0x40000]

    FILE* reg_access_log;

    void init();
    void exit();

    void tick();
};

u8 ibm5150_rb(void* dev, u64 addr);
u16 ibm5150_rw(void* dev, u64 addr);
void ibm5150_wb(void* dev, u64 addr, u8 data);
void ibm5150_ww(void* dev, u64 addr, u16 data);

u8 ibm5150_iorb(void* dev, u16 addr);
u16 ibm5150_iorw(void* dev, u16 addr);
void ibm5150_iowb(void* dev, u16 addr, u8 data);
void ibm5150_ioww(void* dev, u16 addr, u16 data);