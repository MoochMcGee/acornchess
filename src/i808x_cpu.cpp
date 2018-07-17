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
    printf("Opcode:%02x\nCS:%04x\nIP:%04x\nFLAGS:%04x\n", opcode, cs, ip, flags.whole);
    printf("DS:%04x\nES:%04x\nSS:%04x\n", ds, es, ss);
    printf("AX:%04x\nBX:%04x\nCX:%04x\nDX:%04x\nSP:%04x\nBP:%04x\nSI:%04x\nDI:%04x\n", r[0], r[3], r[1], r[2], r[4], r[5], r[6], r[7]);
    ip++;
    switch(opcode)
    {
        case 0x60:
        case 0x70:
        {
            //JO NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(flags.overflow) ip += off;
            break;
        }
        case 0x61:
        case 0x71:
        {
            //JNO NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(!flags.overflow) ip += off;
            break;
        }
        case 0x62:
        case 0x72:
        {
            //JC NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(flags.carry) ip += off;
            break;
        }
        case 0x63:
        case 0x73:
        {
            //JNC NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(!flags.carry) ip += off;
            break;
        }
        case 0x64:
        case 0x74:
        {
            //JZ NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(flags.zero) ip += off;
            break;
        }
        case 0x65:
        case 0x75:
        {
            //JNZ NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(!flags.zero) ip += off;
            break;
        }
        case 0x66:
        case 0x76:
        {
            //JCE NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(flags.carry || flags.zero) ip += off;
            break;
        }
        case 0x67:
        case 0x77:
        {
            //JNCE NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(!(flags.carry || flags.zero)) ip += off;
            break;
        }
        case 0x68:
        case 0x78:
        {
            //JS NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(flags.sign) ip += off;
            break;
        }
        case 0x69:
        case 0x79:
        {
            //JNS NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(!flags.sign) ip += off;
            break;
        }
        case 0x6a:
        case 0x7a:
        {
            //JP NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(flags.parity) ip += off;
            break;
        }
        case 0x6b:
        case 0x7b:
        {
            //JNP NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(!flags.parity) ip += off;
            break;
        }
        case 0x6c:
        case 0x7c:
        {
            //JL NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if((flags.sign != flags.overflow) && !flags.zero) ip += off;
            break;
        }
        case 0x6d:
        case 0x7d:
        {
            //JNL NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if(flags.sign == flags.overflow) ip += off;
            break;
        }
        case 0x6e:
        case 0x7e:
        {
            //JLE NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if((flags.sign != flags.overflow) || flags.zero) ip += off;
            break;
        }
        case 0x6f:
        case 0x7f:
        {
            //JNLE NEAR
            s16 off = (s8)rb(cs, ip);
            ip++;
            if((flags.sign == flags.overflow) && !flags.zero) ip += off;
            break;
        }
        case 0x9e:
        {
            //SAHF
            flags.whole = (flags.whole & 0xff2a) | (r[0].h & 0xd5);
            break;
        }
        case 0x9f:
        {
            //LAHF
            r[0].h = (r[0].h & 0x2a) | ((u8)flags.whole & 0xd7);
            break;
        }
        case 0xb0:
        {
            //MOV AL, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[0].l = temp;
            break;
        }
        case 0xb1:
        {
            //MOV CL, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[1].l = temp;
            break;
        }
        case 0xb2:
        {
            //MOV DL, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[2].l = temp;
            break;
        }
        case 0xb3:
        {
            //MOV BL, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[3].l = temp;
            break;
        }
        case 0xb4:
        {
            //MOV AH, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[0].h = temp;
            break;
        }
        case 0xb5:
        {
            //MOV CH, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[1].h = temp;
            break;
        }
        case 0xb6:
        {
            //MOV DH, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[2].h = temp;
            break;
        }
        case 0xb7:
        {
            //MOV BH, imm8
            u8 temp = rb(cs, ip);
            ip++;
            r[3].h = temp;
            break;
        }
        case 0xea:
        {
            //JMP FAR
            u16 new_ip = rw(cs, ip);
            ip += 2;
            u16 new_cs = rw(cs, ip);
            ip += 2;
            cs = new_cs;
            ip = new_ip;
            break;
        }
        case 0xf8:
        {
            //CLC
            flags.carry = 0;
            break;
        }
        case 0xf9:
        {
            //STC
            flags.carry = 1;
            break;
        }
        case 0xfa:
        {
            //CLI
            flags.interrupt_enable = 0;
            break;
        }
        case 0xfb:
        {
            //STI
            flags.interrupt_enable = 1;
            break;
        }
        case 0xfc:
        {
            //CLD
            flags.direction = 0;
            break;
        }
        case 0xfd:
        {
            //STD
            flags.direction = 1;
            break;
        }
        default:
        {
            printf("Unknown opcode %02x!\n", opcode);
            break;
        }
    }
}

void i808x_cpu::run(int insns)
{
    for(int i = 0; i<insns; i++)
    {
        tick();
    }
}