#pragma once

#include <array>
#include <string>
#include "EProgramMemoryMode.h"
#include "EReadWriteMode.h"

/// <summary>
/// GORBITSA namespace
/// </summary>
namespace GORBITSA
{
	/// <summary>
	/// Virtual machine class
	/// </summary>
	class VM
	{
	private:

		/// <summary>
		/// ROM cells
		/// </summary>
		std::array<std::tuple<uint8_t, uint8_t>, 256> romCells;

		/// <summary>
		/// RAM cells
		/// </summary>
		std::array<uint8_t, 256> ramCells;

		/// <summary>
		/// Program memory mode
		/// </summary>
		EProgramMemoryMode programMemoryMode;

		/// <summary>
		/// Program counter
		/// </summary>
		uint8_t programCounter;

		/// <summary>
		/// X register
		/// </summary>
		uint8_t xRegister;

		/// <summary>
		/// Not halting
		/// </summary>
		bool notHalting;


	public:
		VM() = delete;
		VM(const VM& vm) = delete;
		VM(VM&& vm) = delete;

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="programMemoryMode">Program memory mode</param>
		VM(EProgramMemoryMode programMemoryMode);

		/// <summary>
		/// Destructor
		/// </summary>
		~VM();

		/// <summary>
		/// Load GORBITSA code
		/// </summary>
		/// <param name="input">Input</param>
		/// <returns>"true" if successful, otherwise "false"</returns>
		bool Load(const std::string& input);

		/// <summary>
		/// Clear virtual machine
		/// </summary>
		void Clear();

		/// <summary>
		/// Execute step
		/// </summary>
		/// <param name="readMode">Read mode</param>
		/// <param name="writeMode">Write mode</param>
		/// <returns>"true" if code was executed, otherwise "false"</returns>
		bool Step(EReadWriteMode readMode, EReadWriteMode writeMode);

		/// <summary>
		/// Execute
		/// </summary>
		/// <param name="readMode">Read mode</param>
		/// <param name="writeMode">Write mode</param>
		void Execute(EReadWriteMode readMode, EReadWriteMode writeMode);
	};
}
