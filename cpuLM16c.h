#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Bus;

class cpuLM16c
{
private:
	Bus* bus;

public:
	union CPUReg {
		uint16_t r;
		struct {
			uint8_t lo;
			uint8_t hi;
		} b;
	};
	uint16_t pc, sp;
	CPUReg ra, rb, rc, rd;
	union {
		struct {
			uint8_t Zero : 1;
			uint8_t Sign : 1;
			uint8_t Overflow : 1;
			uint8_t Interrupt : 1;
		};
		uint8_t flags;
	} flags;
	union {
		struct {
			uint8_t Interruptable : 1;
			uint8_t HandleInt : 1;
		};
		uint8_t controls;
	} controls;

	uint8_t opcode, aux;
	uint16_t address;

	uint8_t fetch();
	uint8_t fetched, cycle;

	uint16_t *regs16[4];
	uint8_t* regs8[8];
	std::string mnemonic;

public:
	cpuLM16c();
	~cpuLM16c();
	void ConnectBus(Bus *b);
	void clock();
	void intr();
	void nmi();

private:
	//Load Register Instructions
	void MOVr2r();	void MOVi2r();
	void MOVm2r();	void MOVr2m();
	void MOVr2sp();	void MOVsp2r();
	void MOVb2b();	void MOVi2b();
	void MOVm2b();	void MOVb2m();
	//Stack Instructions
	void PUSH();	void PUSHB();	void PUSHF();
	void POP();		void POPB();	void POPF();
	//Logical Instructions
	void AND();		void OR();		void XOR();		void NOT();
	void ANDI();	void ORI();		void XORI();	void TEST();
	void ANDB();	void ORB();		void XORB();	void NOTB();
	void ANDBI();	void ORBI();	void XORBI();	void TESTB();
	//ALU Instructions
	void INC();		void DEC();		void INCB();	void DECB();
	void ADD();		void ADDB();	void ADDI();	void ADDBI();
	void SUB();		void SUBB();	void SUBI();	void SUBBI();
	//Shifters
	void SHL();		void SHR();		void SAR();
	void ROL();		void ROR();
	//Control Flow Instructions
	void JMP();
	void JZ();		void JS();		void JO();
	void JNZ();		void JNS();		void JNO();
	void CALL();	void INT();
	void RET();		void IRET();
	//Special
	void zIN();		void zOUT();
	void XXX();

private:
	enum Reg16bit {
		RA = 0,
		RB,
		RC,
		RD
	};
	enum Reg8bit {
		AL = 0,
		BL,
		CL,
		DL,
		AH,
		BH,
		CH,
		DH
	};
	struct INSTRUCTION {
		std::string name;
		void (cpuLM16c::* op)(void) = nullptr;
	};
	std::vector<INSTRUCTION> lookup;
};

