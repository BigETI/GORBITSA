#include <iostream>
#include <sstream>
#include "VM.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="programMemoryMode">Program memory mode</param>
GORBITSA::VM::VM(GORBITSA::EProgramMemoryMode programMemoryMode) : programMemoryMode(programMemoryMode), programCounter(0), xRegister(0), notHalting(true)
{
	romCells.fill(std::tuple<uint8_t, uint8_t>(0, 0));
	ramCells.fill(0);
}

/// <summary>
/// Destructor
/// </summary>
GORBITSA::VM::~VM()
{
	Clear();
}

/// <summary>
/// Load GORBITSA code
/// </summary>
/// <param name="input">Input</param>
/// <returns>"true" if successful, otherwise "false"</returns>
bool GORBITSA::VM::Load(const std::string& input)
{
	bool ret(true);
	std::stringstream string_stream(input);
	std::string output;
	std::uint8_t program_counter(0);
	uint8_t instruction;
	uint8_t argument(0);
	bool is_full(false);
	Clear();
	while (string_stream >> output)
	{
		if (is_full)
		{
			ret = false;
			break;
		}
		else
		{
			if (output.length() > 0)
			{
				instruction = output[0];
				switch (output[0])
				{
				case 'G':
				case 'O':
				case 'B':
				case 'I':
				case 'S':
				case 'A':
				case 'g':
				case 'o':
				case 'b':
				case 'i':
				case 's':
				case 'a':
					try
					{
						argument = std::stoi(output.substr(1U));
					}
					catch (std::invalid_argument e)
					{
						ret = false;
						std::cerr << e.what();
					}
					break;
				case 'R':
				case 'T':
				case 'r':
				case 't':
					break;
				default:
					ret = false;
				}
				if (ret)
				{
					if (programMemoryMode == GORBITSA::EProgramMemoryMode::ROM)
					{
						romCells[program_counter] = std::tuple<std::uint8_t, std::uint8_t>(instruction, argument);
						++program_counter;
					}
					else
					{
						ramCells[program_counter] = instruction;
						++program_counter;
						ramCells[program_counter] = argument;
						++program_counter;
					}
				}
				if (!program_counter)
				{
					is_full = true;
				}
				if (!ret)
				{
					break;
				}
			}
		}
	}
	return ret;
}

/// <summary>
/// Clear virtual machine
/// </summary>
void GORBITSA::VM::Clear()
{
	romCells.fill(std::tuple<uint8_t, uint8_t>(0, 0));
	ramCells.fill(0);
	programCounter = 0;
	xRegister = 0;
	notHalting = true;
}

/// <summary>
/// Execute step
/// </summary>
/// <param name="readMode">Read mode</param>
/// <param name="writeMode">Write mode</param>
/// <returns>"true" if code was executed, otherwise "false"</returns>
bool GORBITSA::VM::Step(GORBITSA::EReadWriteMode readMode, GORBITSA::EReadWriteMode writeMode)
{
	bool ret(false);
	if (notHalting)
	{
		std::uint8_t instruction;
		std::uint8_t argument;
		bool increment_program_counter(true);
		if (programMemoryMode == GORBITSA::EProgramMemoryMode::ROM)
		{
			instruction = std::get<0>(romCells[programCounter]);
			argument = std::get<1>(romCells[programCounter]);
		}
		else
		{
			instruction = ramCells[programCounter];
			argument = ramCells[(programCounter + 1) & 0xFF];
		}
		switch (instruction)
		{
		case 'G':
			xRegister = ramCells[argument];
			ret = true;
			break;
		case 'O':
			ramCells[argument] = xRegister;
			ret = true;
			break;
		case 'R':
			switch (readMode)
			{
			case GORBITSA::EReadWriteMode::ASCII:
				xRegister = std::cin.get();
				break;
			case GORBITSA::EReadWriteMode::Number:
				int number;
				std::cin >> number;
				xRegister = number;
				break;
			}
			ret = true;
			break;
		case 'B':
			if (!xRegister)
			{
				programCounter = ((programMemoryMode == GORBITSA::EProgramMemoryMode::ROM) ? argument : (argument * 2));
				increment_program_counter = false;
			}
			ret = true;
			break;
		case 'I':
			xRegister += argument;
			ret = true;
			break;
		case 'T':
			switch (writeMode)
			{
			case GORBITSA::EReadWriteMode::ASCII:
				std::cout << static_cast<char>(xRegister);
				break;
			case GORBITSA::EReadWriteMode::Number:
				std::cout << static_cast<int>(xRegister);
				break;
			}
			ret = true;
			break;
		case 'S':
			xRegister = argument;
			ret = true;
			break;
		case 'A':
			xRegister += ramCells[argument];
			ret = true;
			break;
		case 'g':
			xRegister = ramCells[ramCells[argument]];
			ret = true;
			break;
		case 'o':
			ramCells[ramCells[argument]] = xRegister;
			ret = true;
			break;
		case 'r':
			switch (readMode)
			{
			case GORBITSA::EReadWriteMode::ASCII:
				ramCells[argument] = std::cin.get();
				break;
			case GORBITSA::EReadWriteMode::Number:
				int number;
				std::cin >> number;
				ramCells[argument] = number;
				break;
			}
			ret = true;
			break;
		case 'b':
			if (!xRegister)
			{
				programCounter = ((programMemoryMode == GORBITSA::EProgramMemoryMode::ROM) ? ramCells[argument] : (ramCells[argument] * 2));
				increment_program_counter = false;
			}
			ret = true;
			break;
		case 'i':
			ramCells[argument] += xRegister;
			ret = true;
			break;
		case 't':
			switch (writeMode)
			{
			case GORBITSA::EReadWriteMode::ASCII:
				std::cout << static_cast<char>(ramCells[argument]);
				break;
			case GORBITSA::EReadWriteMode::Number:
				std::cout << static_cast<int>(ramCells[argument]);
				break;
			}
			ret = true;
			break;
		case 's':
			xRegister ^= ramCells[argument];
			ret = true;
			break;
		case 'a':
			xRegister += ramCells[ramCells[argument]];
			ret = true;
			break;
		default:
			increment_program_counter = false;
		}
		if (increment_program_counter)
		{
			if (programMemoryMode == GORBITSA::EProgramMemoryMode::ROM)
			{
				++programCounter;
			}
			else
			{
				programCounter += 2;
			}
			if (!programCounter)
			{
				notHalting = false;
			}
		}
	}
	return ret;
}

/// <summary>
/// Execute
/// </summary>
/// <param name="readMode">Read mode</param>
/// <param name="writeMode">Read mode</param>
void GORBITSA::VM::Execute(GORBITSA::EReadWriteMode readMode, GORBITSA::EReadWriteMode writeMode)
{
	while (Step(readMode, writeMode));
}
