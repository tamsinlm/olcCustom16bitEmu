#include "cpuLM16c.h"
#include "Bus.h"

#define left8(x) (x&0x7)
#define left16(x) (x&0x3)
#define right8(x) ((x>>3)&0x7)
#define right16(x) ((x>>2)&0x3)

uint8_t cpuLM16c::fetch()
{
	fetched = bus->read(pc);
	pc++;
	return fetched;
}

cpuLM16c::cpuLM16c()
{
	bus = NULL;
	pc = 0xFF00;
	ra.r = rb.r = rc.r = rd.r = 0;
	opcode = aux = fetched = 0;
	address = 0;
	sp = 0x0FF;
	flags.flags = 0;
	controls.controls = 0x01;
	cycle = 0;
	regs16[0] = &ra.r;
	regs16[1] = &rb.r;
	regs16[2] = &rc.r;
	regs16[3] = &rd.r;
	regs8[0] = &ra.b.lo;
	regs8[1] = &rb.b.lo;
	regs8[2] = &rc.b.lo;
	regs8[3] = &rd.b.lo;
	regs8[4] = &ra.b.hi;
	regs8[5] = &rb.b.hi;
	regs8[6] = &rc.b.hi;
	regs8[7] = &rd.b.hi;
	using a = cpuLM16c;
	lookup =
	{
		//0000
		{"mov ra, ra",&a::MOVr2r},{"mov ra, rb",&a::MOVr2r},{"mov ra, rc",&a::MOVr2r},{"mov ra, rd",&a::MOVr2r},//00
		{"mov rb, ra",&a::MOVr2r},{"mov rb, rb",&a::MOVr2r},{"mov rb, rc",&a::MOVr2r},{"mov rb, rd",&a::MOVr2r},//01
		{"mov rc, ra",&a::MOVr2r},{"mov rc, rb",&a::MOVr2r},{"mov rc, rc",&a::MOVr2r},{"mov rc, rd",&a::MOVr2r},//10
		{"mov rd, ra",&a::MOVr2r},{"mov rd, rb",&a::MOVr2r},{"mov rd, rc",&a::MOVr2r},{"mov rd, rd",&a::MOVr2r},//11
		//0001
		{"mov ra",&a::MOVi2r},{"mov rb",&a::MOVi2r},{"mov rc",&a::MOVi2r},{"mov rd",&a::MOVi2r},//00
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//01
		{"mov ra",&a::MOVm2r},{"mov rb",&a::MOVm2r},{"mov rc",&a::MOVm2r},{"mov rd",&a::MOVm2r},//10
		{"mov ra",&a::MOVr2m},{"mov rb",&a::MOVr2m},{"mov rc",&a::MOVr2m},{"mov rd",&a::MOVr2m},//11
		//0010
		{"mov ra, sp",&a::MOVsp2r},{"mov rb, sp",&a::MOVsp2r},{"mov rc, sp",&a::MOVsp2r},{"mov rd, sp",&a::MOVsp2r},//00
		{"mov sp, ra",&a::MOVr2sp},{"mov sp, rb",&a::MOVr2sp},{"mov sp, rc",&a::MOVr2sp},{"mov sp, rd",&a::MOVr2sp},//01
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//10
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//11
		//0011
		{"mov",&a::MOVb2b},{},{},{},//00
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//01
		{"mov al",&a::MOVi2b},{"mov bl",&a::MOVi2b},{"mov cl",&a::MOVi2b},{"mov dl",&a::MOVi2b},//10
		{"mov ah",&a::MOVi2b},{"mov bh",&a::MOVi2b},{"mov ch",&a::MOVi2b},{"mov dh",&a::MOVi2b},//11
		//0100
		{"mov al",&a::MOVm2b},{"mov bl",&a::MOVm2b},{"mov cl",&a::MOVm2b},{"mov dl",&a::MOVm2b},//00
		{"mov ah",&a::MOVm2b},{"mov bh",&a::MOVm2b},{"mov ch",&a::MOVm2b},{"mov dh",&a::MOVm2b},//01
		{"mov al",&a::MOVb2m},{"mov bl",&a::MOVb2m},{"mov cl",&a::MOVb2m},{"mov dl",&a::MOVb2m},//10
		{"mov ah",&a::MOVb2m},{"mov bh",&a::MOVb2m},{"mov ch",&a::MOVb2m},{"mov dh",&a::MOVb2m},//11
		//0101
		{"push ra",&a::PUSH},{"push rb",&a::PUSH},{"push rc",&a::PUSH},{"push rd",&a::PUSH},//00
		{"pushf",&a::PUSHF},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//01
		{"push al",&a::PUSHB},{"push bl",&a::PUSHB},{"push cl",&a::PUSHB},{"push dl",&a::PUSHB},//10
		{"push ah",&a::PUSHB},{"push bh",&a::PUSHB},{"push ch",&a::PUSHB},{"push dh",&a::PUSHB},//11
		//0110
		{"pop ra",&a::POP},{"pop rb",&a::POP},{"pop rc",&a::POP},{"pop rd",&a::POP},//00
		{"popf",&a::POPF},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//01
		{"pop al",&a::POPB},{"pop bl",&a::POPB},{"pop cl",&a::POPB},{"pop dl",&a::POPB},//10
		{"pop ah",&a::POPB},{"pop bh",&a::POPB},{"pop ch",&a::POPB},{"pop dh",&a::POPB},//11
		//0111
		{"and",&a::AND},{"or",&a::OR},{"xor",&a::XOR},{"not",&a::NOT},
		{"andi",&a::ANDI},{"ori",&a::ORI},{"xori",&a::XORI},{"???",&a::XXX},
		{"andb",&a::ANDB},{"orb",&a::ORB},{"xorb",&a::XORB},{"notb",&a::NOTB},
		{"andbi",&a::ANDBI},{"orbi",&a::ORBI},{"xorbi",&a::XORBI},{"???",&a::XXX},
		//1000
		{"inc ra",&a::INC},{"inc rb",&a::INC},{"inc rc",&a::INC},{"inc rd",&a::INC},//00
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//11
		{"dec ra",&a::DEC},{"dec rb",&a::DEC},{"dec rc",&a::DEC},{"dec rd",&a::DEC},//01
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//11
		//1001
		{"inc al",&a::INCB},{"inc bl",&a::INCB},{"inc cl",&a::INCB},{"inc dl",&a::INCB},//00
		{"inc ah",&a::INCB},{"inc bh",&a::INCB},{"inc ch",&a::INCB},{"inc dh",&a::INCB},//01
		{"dec al",&a::DECB},{"dec bl",&a::DECB},{"dec cl",&a::DECB},{"dec dl",&a::DECB},//10
		{"dec ah",&a::DECB},{"dec bh",&a::DECB},{"dec ch",&a::DECB},{"dec dh",&a::DECB},//11
		//1010
		{"add ra, ra",&a::ADD},{"add ra, rb",&a::ADD},{"add ra, rc",&a::ADD},{"add ra, rd",&a::ADD},
		{"add rb, ra",&a::ADD},{"add rb, rb",&a::ADD},{"add rb, rc",&a::ADD},{"add rb, rd",&a::ADD},
		{"add rc, ra",&a::ADD},{"add rc, rb",&a::ADD},{"add rc, rc",&a::ADD},{"add rc, rd",&a::ADD},
		{"add rd, ra",&a::ADD},{"add rd, rb",&a::ADD},{"add rd, rc",&a::ADD},{"add rd, rd",&a::ADD},
		//1011
		{"addi ra",&a::ADDI},{"addi rb",&a::ADDI},{"addi rc",&a::ADDI},{"addi rd",&a::ADDI},
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"addb",&a::ADDB},
		{"addbi al",&a::ADDBI},{"addbi bl",&a::ADDBI},{"addbi cl",&a::ADDBI},{"addbi dl",&a::ADDBI},
		{"addbi ah",&a::ADDBI},{"addbi bh",&a::ADDBI},{"addbi ch",&a::ADDBI},{"addbi dh",&a::ADDBI},
		//1100
		{"sub ra, ra",&a::SUB},{"sub ra, rb",&a::SUB},{"sub ra, rc",&a::SUB},{"sub ra, rd",&a::SUB},
		{"sub rb, ra",&a::SUB},{"sub rb, rb",&a::SUB},{"sub rb, rc",&a::SUB},{"sub rb, rd",&a::SUB},
		{"sub rc, ra",&a::SUB},{"sub rc, rb",&a::SUB},{"sub rc, rc",&a::SUB},{"sub rc, rd",&a::SUB},
		{"sub rd, ra",&a::SUB},{"sub rd, rb",&a::SUB},{"sub rd, rc",&a::SUB},{"sub rd, rd",&a::SUB},
		//1101
		{"subi ra",&a::SUBI},{"subi rb",&a::SUBI},{"subi rc",&a::SUBI},{"subi rd",&a::SUBI},
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"subb",&a::SUBB},
		{"subbi al",&a::SUBBI},{"subbi bl",&a::SUBBI},{"subbi cl",&a::SUBBI},{"subbi dl",&a::SUBBI},
		{"subbi ah",&a::SUBBI},{"subbi bh",&a::SUBBI},{"subbi ch",&a::SUBBI},{"subbi dh",&a::SUBBI},
		//1110
		{"shl",&a::SHL},{"shr",&a::SHR},{"sal",&a::SHL},{"sar",&a::SAR},//10
		{"rol",&a::ROL},{"ror",&a::ROR},{"???",&a::XXX},{"???",&a::XXX},//10
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//11
		{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},{"???",&a::XXX},//11
		//1111
		{"jmp",&a::JMP},{"jz",&a::JZ},{"js",&a::JS},{"jo",&a::JO},//00
		{"???",&a::XXX},{"jnz",&a::JNZ},{"jns",&a::JNS},{"jno",&a::JNO},//00
		{"call",&a::CALL},{"int",&a::INT},{"ret",&a::RET},{"iret",&a::IRET},//01
		{"in",&a::zIN},{"out",&a::zOUT},{"???",&a::XXX},{"???",&a::XXX},//10
	};
}

cpuLM16c::~cpuLM16c()
{
}

void cpuLM16c::ConnectBus(Bus* b)
{
	bus = b;
}

void cpuLM16c::clock()
{
	if (cycle == 0)
	{
		/*if (controls.Interruptable && flags.Interrupt)
		{
			controls.Interruptable = 0;
		}
		else*/
			opcode = fetch();
			mnemonic = this->lookup[opcode].name;
	}
	else
		(this->*lookup[opcode].op)();
	cycle++;
}

void cpuLM16c::intr()
{
	if (controls.Interruptable)
		flags.Interrupt = 1;
}

void cpuLM16c::nmi()
{
	flags.Interrupt = 1;
}

void cpuLM16c::MOVr2r()
{
	*regs16[left16(opcode)] = *regs16[right16(opcode)];
	cycle = -1;
}

void cpuLM16c::MOVi2r()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else
	{
		*regs16[left16(opcode)] = address;
		cycle = -1;
	}
}

void cpuLM16c::MOVm2r()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else if (cycle == 3)
		*regs16[left16(opcode)] = bus->read(address++);
	else
	{
		*regs16[left16(opcode)] |= bus->read(address) << 8;
		cycle = -1;
	}
}

void cpuLM16c::MOVr2m()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else if (cycle == 3)
		bus->write(address++, (uint8_t)(*regs16[left16(opcode)]));
	else
	{
		bus->write(address, (uint8_t)(*regs16[left16(opcode)] >> 8));
		cycle = -1;
	}
}

void cpuLM16c::MOVr2sp()
{
	sp = *regs16[left16(opcode)];
	cycle = -1;
}

void cpuLM16c::MOVsp2r()
{
	*regs16[left16(opcode)] = sp;
	cycle = -1;
}

void cpuLM16c::MOVb2b()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs8[left8(aux)] = *regs8[right8(aux)];
		cycle = -1;
	}
}

void cpuLM16c::MOVi2b()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		*regs8[left8(opcode)] = (uint8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::MOVm2b()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else
	{
		*regs8[left8(opcode)] = bus->read(address);
		cycle = -1;
	}
}

void cpuLM16c::MOVb2m()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else
	{
		bus->write(address, (uint8_t)(*regs8[left8(opcode)]));
		cycle = -1;
	}
}

void cpuLM16c::PUSH()
{
	if (cycle == 1)
		bus->write(sp--, (uint8_t)*regs16[left16(opcode)]);
	else
	{
		bus->write(sp--, *regs16[left16(opcode)] >> 8);
		cycle = -1;
	}
}

void cpuLM16c::PUSHB()
{
	bus->write(sp--, *regs8[left8(opcode)]);
	cycle = -1;
}

void cpuLM16c::PUSHF()
{
	bus->write(sp--, flags.flags);
	cycle = -1;
}

void cpuLM16c::POP()
{
	if (cycle == 1)
		*regs16[left16(opcode)] = bus->read(sp++) << 8;
	else
	{
		*regs16[left16(opcode)] |= bus->read(sp++);
		cycle = -1;
	}
}

void cpuLM16c::POPB()
{
	*regs8[left8(opcode)] = bus->read(sp++);
	cycle = -1;
}

void cpuLM16c::POPF()
{
	flags.flags = bus->read(sp++);
	cycle = -1;
}

void cpuLM16c::AND()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs16[left16(aux)] = *regs16[left16(aux)] & *regs16[right16(aux)];
		flags.Zero = *regs16[left16(aux)] == 0;
		flags.Sign = *regs16[left16(aux)] >> 15;
		cycle = -1;
	}
}

void cpuLM16c::OR()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs16[left16(aux)] = *regs16[left16(aux)] | *regs16[right16(aux)];
		flags.Zero = *regs16[left16(aux)] == 0;
		flags.Sign = *regs16[left16(aux)] >> 15;
		cycle = -1;
	}
}

void cpuLM16c::XOR()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs16[left16(aux)] = *regs16[left16(aux)] ^ *regs16[right16(aux)];
		flags.Zero = *regs16[left16(aux)] == 0;
		flags.Sign = *regs16[left16(aux)] >> 15;
		cycle = -1;
	}
}

void cpuLM16c::NOT()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs16[left16(aux)] = ~*regs16[left16(aux)];
		flags.Zero = *regs16[left16(aux)] == 0;
		flags.Sign = *regs16[left16(aux)] >> 15;
		cycle = -1;
	}
}

void cpuLM16c::ANDI()
{
	if (cycle == 1)
		aux = fetch();
	else if (cycle == 1)
		address = fetch();
	else if (cycle == 1)
		address |= fetch() << 8;
	else
	{
		*regs16[left16(aux)] = *regs16[left16(aux)] & address;
		flags.Zero = *regs16[left16(aux)] == 0;
		flags.Sign = *regs16[left16(aux)] >> 15;
		cycle = -1;
	}
}

void cpuLM16c::ORI()
{
	if (cycle == 1)
		aux = fetch();
	else if (cycle == 1)
		address = fetch();
	else if (cycle == 1)
		address |= fetch() << 8;
	else
	{
		*regs16[left16(aux)] = *regs16[left16(aux)] | address;
		flags.Zero = *regs16[left16(aux)] == 0;
		flags.Sign = *regs16[left16(aux)] >> 15;
		cycle = -1;
	}
}

void cpuLM16c::XORI()
{
	if (cycle == 1)
		aux = fetch();
	else if (cycle == 1)
		address = fetch();
	else if (cycle == 1)
		address |= fetch() << 8;
	else
	{
		*regs16[left16(aux)] = *regs16[left16(aux)] ^ address;
		flags.Zero = *regs16[left16(aux)] == 0;
		flags.Sign = *regs16[left16(aux)] >> 15;
		cycle = -1;
	}
}

void cpuLM16c::TEST()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint16_t res = *regs16[left16(aux)] - *regs16[right16(aux)];
		flags.Zero = res == 0;
		flags.Sign = res >> 15;
		flags.Overflow = *regs16[left16(aux)] < res;
		cycle = -1;
	}
}

void cpuLM16c::ANDB()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs8[left8(aux)] = *regs8[left8(aux)] & *regs8[right8(aux)];
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		cycle = -1;
	}
}

void cpuLM16c::ORB()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs8[left8(aux)] = *regs8[left8(aux)] | *regs8[right8(aux)];
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		cycle = -1;
	}
}

void cpuLM16c::XORB()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs8[left8(aux)] = *regs8[left8(aux)] ^ *regs8[right8(aux)];
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		cycle = -1;
	}
}

void cpuLM16c::NOTB()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		*regs8[left8(aux)] = ~*regs8[left8(aux)];
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		cycle = -1;
	}
}

void cpuLM16c::ANDBI()
{
	if (cycle == 1)
		aux = fetch();
	else if (cycle == 1)
		address = fetch();
	else
	{
		*regs8[left8(aux)] = *regs8[left8(aux)] & (uint8_t)address;
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		cycle = -1;
	}
}

void cpuLM16c::ORBI()
{
	if (cycle == 1)
		aux = fetch();
	else if (cycle == 1)
		address = fetch();
	else
	{
		*regs8[left8(aux)] = *regs8[left8(aux)] | (uint8_t)address;
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		cycle = -1;
	}
}

void cpuLM16c::XORBI()
{
	if (cycle == 1)
		aux = fetch();
	else if (cycle == 1)
		address = fetch();
	else
	{
		*regs8[left8(aux)] = *regs8[left8(aux)] ^ (uint8_t)address;
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		cycle = -1;
	}
}

void cpuLM16c::TESTB()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t res = *regs8[left8(aux)] - *regs8[right8(aux)];
		flags.Zero = res == 0;
		flags.Sign = res >> 7;
		flags.Overflow = *regs8[left8(aux)] < res;
		cycle = -1;
	}
}

void cpuLM16c::INC()
{
	(*regs16[left16(opcode)])++;
	flags.Zero = *regs16[left16(opcode)] == 0;
	flags.Sign = *regs16[left16(opcode)] >> 15;
	flags.Overflow = *regs16[left16(opcode)] == 0;
	cycle = -1;
}

void cpuLM16c::DEC()
{
	(*regs16[left16(opcode)])--;
	flags.Zero = *regs16[left16(opcode)] == 0;
	flags.Sign = *regs16[left16(opcode)] >> 15;
	flags.Overflow = *regs16[left16(opcode)] == 0xFFFF;
	cycle = -1;
}

void cpuLM16c::INCB()
{
	(*regs8[left8(opcode)])++;
	flags.Zero = *regs8[left8(opcode)] == 0;
	flags.Sign = *regs8[left8(opcode)] >> 7;
	flags.Overflow = *regs8[left8(opcode)] == 0;
	cycle = -1;
}

void cpuLM16c::DECB()
{
	(*regs8[left8(opcode)])--;
	flags.Zero = *regs8[left8(opcode)] == 0;
	flags.Sign = *regs8[left8(opcode)] >> 7;
	flags.Overflow = *regs8[left8(opcode)] == 0xFF;
	cycle = -1;
}

void cpuLM16c::ADD()
{
	uint16_t prev = *regs16[left16(opcode)];
	*regs16[left16(opcode)] += *regs16[right16(opcode)];
	flags.Zero = *regs16[left16(opcode)] == 0;
	flags.Sign = *regs16[left16(opcode)] >> 15;
	flags.Overflow = *regs16[left16(opcode)] < prev;
	cycle = -1;
}

void cpuLM16c::ADDB()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t prev = *regs8[left8(aux)];
		*regs8[left8(aux)] += *regs8[right8(aux)];
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		flags.Overflow = *regs8[left8(aux)] < prev;
		cycle = -1;
	}
}

void cpuLM16c::ADDI()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else
	{
		uint16_t prev = *regs16[left16(opcode)];
		*regs16[left16(opcode)] += address;
		flags.Zero = *regs16[left16(opcode)] == 0;
		flags.Sign = *regs16[left16(opcode)] >> 15;
		flags.Overflow = *regs16[left16(opcode)] < prev;
		cycle = -1;
	}
}

void cpuLM16c::ADDBI()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t prev = *regs8[left8(opcode)];
		*regs8[left8(opcode)] += aux;
		flags.Zero = *regs8[left8(opcode)] == 0;
		flags.Sign = *regs8[left8(opcode)] >> 7;
		flags.Overflow = *regs8[left8(opcode)] < prev;
		cycle = -1;
	}
}

void cpuLM16c::SUB()
{
	uint16_t prev = *regs16[left16(opcode)];
	*regs16[left16(opcode)] -= *regs16[right16(opcode)];
	flags.Zero = *regs16[left16(opcode)] == 0;
	flags.Sign = *regs16[left16(opcode)] >> 15;
	flags.Overflow = *regs16[left16(opcode)] > prev;
	cycle = -1;
}

void cpuLM16c::SUBB()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t prev = *regs8[left8(aux)];
		*regs8[left8(aux)] -= *regs8[right8(aux)];
		flags.Zero = *regs8[left8(aux)] == 0;
		flags.Sign = *regs8[left8(aux)] >> 7;
		flags.Overflow = *regs8[left8(aux)] > prev;
		cycle = -1;
	}
}

void cpuLM16c::SUBI()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else
	{
		uint16_t prev = *regs16[left16(opcode)];
		*regs16[left16(opcode)] -= address;
		flags.Zero = *regs16[left16(opcode)] == 0;
		flags.Sign = *regs16[left16(opcode)] >> 15;
		flags.Overflow = *regs16[left16(opcode)] > prev;
		cycle = -1;
	}
}

void cpuLM16c::SUBBI()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t prev = *regs8[left8(opcode)];
		*regs8[left8(opcode)] -= aux;
		flags.Zero = *regs8[left8(opcode)] == 0;
		flags.Sign = *regs8[left8(opcode)] >> 7;
		flags.Overflow = *regs8[left8(opcode)] > prev;
		cycle = -1;
	}
}

void cpuLM16c::SHL()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		if (aux & 0x08)
			*regs16[left16(aux)] <<= aux >> 4;
		else
			*regs8[left8(aux)] <<= aux >> 4;
		cycle = -1;
	}
}

void cpuLM16c::SHR()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		if (aux & 0x08)
			*regs16[left16(aux)] >>= aux >> 4;
		else
			*regs8[left8(aux)] >>= aux >> 4;
		cycle = -1;
	}
}

void cpuLM16c::SAR()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t dist = aux >> 4;
		if (aux & 0x08)
		{
			uint16_t lead = *regs16[left16(aux)] & 0x8000 ? 0xFFFF : 0;
			*regs16[left16(aux)] = ((*regs16[left16(aux)]) >> dist) | (lead << (16 - dist));
		}
		else
		{
			dist &= 0x7;
			uint8_t lead = *regs8[left8(aux)] & 0x80 ? 0xFF : 0;
			*regs8[left8(aux)] = ((*regs8[left8(aux)]) >> dist) | (lead << (8 - dist));
		}
		cycle = -1;
	}
}

void cpuLM16c::ROL()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t dist = aux >> 4;
		if (aux & 0x08)
			*regs16[left16(aux)] = ((*regs16[left16(aux)]) << dist) | ((*regs16[left16(aux)]) >> (16 - dist));
		else
		{
			dist &= 0x7;
			*regs8[left8(aux)] = ((*regs8[left8(aux)]) << dist) | ((*regs8[left8(aux)]) >> (8 - dist));
		}
		cycle = -1;
	}
}

void cpuLM16c::ROR()
{
	if (cycle == 1)
		aux = fetch();
	else
	{
		uint8_t dist = aux >> 4;
		if (aux & 0x08)
			*regs16[left16(aux)] = ((*regs16[left16(aux)]) >> dist) | ((*regs16[left16(aux)]) << (16 - dist));
		else
		{
			dist &= 0x07;
			*regs8[left8(aux)] = ((*regs8[left8(aux)]) >> dist) | ((*regs8[left8(aux)]) << (8 - dist));
		}
		cycle = -1;
	}
}

void cpuLM16c::JMP()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		pc += (int8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::JZ()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		if (flags.Zero)
			pc += (int8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::JS()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		if (flags.Sign)
			pc += (int8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::JO()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		if (flags.Overflow)
			pc += (int8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::JNZ()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		if (!flags.Zero)
			pc += (int8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::JNS()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		if (!flags.Sign)
			pc += (int8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::JNO()
{
	if (cycle == 1)
		address = fetch();
	else
	{
		if (!flags.Overflow)
			pc += (int8_t)address;
		cycle = -1;
	}
}

void cpuLM16c::CALL()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else if (cycle == 3)
		bus->write(sp--, (uint8_t)pc);
	else if (cycle == 4)
		bus->write(sp--, (uint8_t)(pc >> 8));
	else
	{
		pc = address;
		cycle = -1;
	}
}

void cpuLM16c::INT()
{
	if (cycle == 1)
		aux = fetch();
	else if (cycle == 2)
		bus->write(sp--, (uint8_t)pc);
	else if (cycle == 3)
		bus->write(sp--, (uint8_t)(pc >> 8));
	else if (cycle == 4)
		address = bus->read(aux << 1);
	else
	{
		address |= bus->read((aux << 1) + 1);
		pc = address;
		flags.Interrupt = 1;
		cycle = -1;
	}
}

void cpuLM16c::RET()
{
	if (cycle == 1)
		address = bus->read(sp++) << 8;
	if (cycle == 2)
		address |= bus->read(sp++);
	else
	{
		pc = address;
		cycle = -1;
	}
}

void cpuLM16c::IRET()
{
	if (cycle == 1)
		address = bus->read(sp++) << 8;
	if (cycle == 2)
		address |= bus->read(sp++);
	else
	{
		pc = address;
		flags.Interrupt = 0;
		cycle = -1;
	}
}

void cpuLM16c::zIN()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else
	{
		ra.b.lo = bus->in(address);
		cycle = -1;
	}
}

void cpuLM16c::zOUT()
{
	if (cycle == 1)
		address = fetch();
	else if (cycle == 2)
		address |= fetch() << 8;
	else
	{
		bus->out(address, ra.b.lo);
		cycle = -1;
	}
}

void cpuLM16c::XXX()
{
	cycle = -1;
}

