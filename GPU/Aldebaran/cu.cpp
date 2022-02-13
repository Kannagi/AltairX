#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <cu.hpp>
#include <gpu.hpp>
#include <iostream>
#include <string_view>

namespace
{
template <size_t N>
struct StringLiteral
{
	constexpr StringLiteral(const char (&str)[N])
	{
		std::copy_n(str, N, value);
	}

	char value[N];
};

uint32_t readbits(uint32_t buffer, uint32_t index, uint32_t size)
{
	return (buffer >> index) & ((1u << size) - 1);
}
}  // namespace

template <StringLiteral NameT, uint32_t OffsetT, uint32_t SizeT>
struct Argument
{
	constexpr static std::string_view name{NameT.value};
	constexpr static uint32_t offset{OffsetT};
	constexpr static uint32_t size{SizeT};

	constexpr static uint32_t get(uint32_t opcode)
	{
		return readbits(opcode, offset, size);
	}
};

template <StringLiteral NameT, uint32_t UnitOpT, typename... ArgumentsT>
struct Opcode
{
	template <size_t i>
	using ArgAt = std::tuple_element_t<i, std::tuple<ArgumentsT...>>;

	constexpr static std::string_view name{NameT.value};
	constexpr static uint32_t unitOp{UnitOpT};
	constexpr static size_t argsCount{sizeof...(ArgumentsT)};

	constexpr static bool match(uint32_t opcode)
	{
		// std::cout << "    " << name << "::match(" << opcode << ")\n";
		// std::cout << "    unit   " << unit << "\n";
		// std::cout << "    unitOp " << unitOp << "\n";
		// std::cout << "    " << (unit | (unitOp << 4))  << " == " << (opcode
		// & 0b11111111) << "\n";
		return (unitOp << 4) == (opcode & 0b11110000);
	}

	template <typename FuncT>
	constexpr static uint32_t dispatch(ComputeUnit_t& cu,
	                                   uint32_t opcode,
	                                   FuncT f)
	{
		for (size_t i{0}; i < N_SHADER; ++i)
		{
			uint32_t res = f(cu, opcode, i);
			if (res != AR_SUCCESS)
				return res;
		}
		return AR_SUCCESS;
	}
};

template <StringLiteral NameT, uint32_t UnitT, typename... OpcodesT>
struct Unit
{
	template <size_t i>
	using OpcodeAt = std::tuple_element_t<i, std::tuple<OpcodesT...>>;

	constexpr static std::string_view name{NameT.value};
	constexpr static uint32_t unit{UnitT};
	constexpr static size_t opcodesCount{sizeof...(OpcodesT)};

	constexpr static bool match(uint32_t opcode)
	{
		// std::cout << "    " << name << "::match(" << opcode << ")\n";
		// std::cout << "    unit   " << unit << "\n";
		// std::cout << "    unitOp " << unitOp << "\n";
		// std::cout << "    " << (unit | (unitOp << 4))  << " == " << (opcode
		// & 0b11111111) << "\n";
		return unit == (opcode & 0b00001111);
	}

	template <typename Opcode>
	static uint32_t seekAndExecute(ComputeUnit_t& cu, uint32_t opcode)
	{
		if (Opcode::match(opcode))
			return Opcode::execute(cu, opcode);
		return AK1_OPCODE_UNKNOWN;
	}

	template <typename Opcode, typename Opcode2, typename... Rest>
	static uint32_t seekAndExecute(ComputeUnit_t& cu, uint32_t opcode)
	{
		if (Opcode::match(opcode))
			return seekAndExecute<Opcode>(cu, opcode);
		return seekAndExecute<Opcode2, Rest...>(cu, opcode);
	}

	constexpr static uint32_t execute(ComputeUnit_t& cu, uint32_t opcode)
	{
		return seekAndExecute<OpcodesT...>(cu, opcode);
	}
};

template <typename... UnitsT>
struct UnitsGroup
{
	template <typename Unit>
	static uint32_t seekAndExecute(ComputeUnit_t& cu, uint32_t opcode)
	{
		if (Unit::match(opcode))
			return Unit::execute(cu, opcode);
		return AK1_OPCODE_UNKNOWN;
	}

	template <typename Unit, typename Unit2, typename... Rest>
	static uint32_t seekAndExecute(ComputeUnit_t& cu, uint32_t opcode)
	{
		if (Unit::match(opcode))
			return seekAndExecute<Unit>(cu, opcode);
		return seekAndExecute<Unit2, Rest...>(cu, opcode);
	}

	static uint32_t execute(ComputeUnit_t& cu, uint32_t opcode)
	{
		return seekAndExecute<UnitsT...>(cu, opcode);
	}
};

template <typename Group1T, typename Group2T>
struct ISA
{
	constexpr static uint32_t execute(ComputeUnit_t& cu,
	                                  uint32_t opcode1,
	                                  uint32_t opcode2)
	{
		auto res = Group1T::execute(cu, opcode1);
		if (res != AR_SUCCESS)
			return res;
		return Group2T::execute(cu, opcode2);
	}
};

namespace opcodes
{
struct ADD : Opcode<"ADD",
                    0,
                    Argument<"Register C", 14, 6>,
                    Argument<"Register B", 20, 6>,
                    Argument<"Register A", 26, 6>>
{
	static uint32_t process(ComputeUnit_t& cu,
	                        uint32_t opcode,
	                        size_t shaderIndex)
	{
		const auto rA = Opcode::ArgAt<2>::get(opcode);
		const auto rB = Opcode::ArgAt<1>::get(opcode);
		const auto rC = Opcode::ArgAt<0>::get(opcode);
		auto& shader  = cu.shaders[shaderIndex];
		auto& vreg    = shader.vreg;
		auto& regA    = vreg[rA];
		auto& regB    = vreg[rB];
		auto& regC    = vreg[rC];

		regA.x() = regB.x() + regC.x();
		regA.y() = regB.y() + regC.y();
		regA.z() = regB.z() + regC.z();
		regA.w() = regB.w() + regC.w();

		return AR_SUCCESS;
	}
	static uint32_t execute(ComputeUnit_t& cu, uint32_t opcode)
	{
		std::cout << Opcode::name << "\n";
		std::cout << "    " << Opcode::ArgAt<0>::name << " "
		          << Opcode::ArgAt<0>::get(opcode) << "\n";
		std::cout << "    " << Opcode::ArgAt<1>::name << " "
		          << Opcode::ArgAt<1>::get(opcode) << "\n";
		std::cout << "    " << Opcode::ArgAt<2>::name << " "
		          << Opcode::ArgAt<2>::get(opcode) << "\n";

		return Opcode::dispatch(cu, opcode, &process);
	}
};

struct NOP : Opcode<"NOP", 0>
{
	static uint32_t execute(ComputeUnit_t& cu, uint32_t opcode)
	{
		std::cout << Opcode::name << "\n";
		return AR_SUCCESS;
	}
};

struct RET : Opcode<"RET", 5>
{
	static uint32_t execute(ComputeUnit_t& cu, uint32_t opcode)
	{
		std::cout << Opcode::name << "\n";
		return AR_END_OF_CODE;
	}
};
}  // namespace opcodes

namespace units
{
using ALU_A = Unit<"ALU_A", 1, opcodes::ADD>;
using ALU_B = Unit<"ALU_B", 0, opcodes::NOP>;
using CMP   = Unit<"CMP", 4, opcodes::RET>;
}  // namespace units

// int ComputeUnit_t::decode()
//{
//	int res = decodeInstruction1();
//	if (res != 0)
//		return res;
//	return decodeInstruction2();
//}

using UnitsGroup1 = UnitsGroup<units::ALU_A, units::ALU_B, units::CMP>;
using UnitsGroup2 = UnitsGroup<units::ALU_A, units::ALU_B, units::CMP>;
using GPUISA      = ISA<UnitsGroup1, UnitsGroup2>;

uint32_t ComputeUnit_t::execute()
{
	pc = reinterpret_cast<const uint32_t*>(gpu.pc);

	uint32_t res = AR_END_OF_CODE;
	do
	{
		res = GPUISA::execute(*this, pc[0], pc[1]);
		pc += 2;
	} while (res != AR_END_OF_CODE                   //
	         && res != AR_ERROR_ILLEGAL_INSTRUCTION  //
	         && res != AK1_OPCODE_UNKNOWN);

	return res;
}
