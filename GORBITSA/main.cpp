#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "VM.h"
#include "EParseCommandLineMode.h"

/// <summary>
/// Main entry point
/// </summary>
/// <param name="argc">Argument count</param>
/// <param name="argv">Arguments</param>
/// <returns>Exit code</returns>
int main(int argc, char* argv[])
{
	int ret(0);
	std::vector<std::string> file_paths;
	bool success(true);
	GORBITSA::EProgramMemoryMode program_memory_mode(GORBITSA::EProgramMemoryMode::ROM);
	GORBITSA::EReadWriteMode read_mode(GORBITSA::EReadWriteMode::ASCII);
	GORBITSA::EReadWriteMode write_mode(GORBITSA::EReadWriteMode::ASCII);
	GORBITSA::EParseCommandLineMode parse_command_line_mode(GORBITSA::EParseCommandLineMode::Nothing);
	std::string argument;
	std::string key;
	for (int index = 1; index < argc; index++)
	{
		argument = argv[index];
		if (argument.length() > 0)
		{
			switch (parse_command_line_mode)
			{
			case GORBITSA::EParseCommandLineMode::Nothing:
				if (argument[0] == '-')
				{
					key = argument;
					std::transform(key.begin(), key.end(), key.begin(), tolower);
					if ((key == "-pmm") || (key == "--program-memory-mode"))
					{
						parse_command_line_mode = GORBITSA::EParseCommandLineMode::ProgramMemoryMode;
					}
					else if ((key == "-rm") || (key == "--read-mode"))
					{
						parse_command_line_mode = GORBITSA::EParseCommandLineMode::ReadMode;
					}
					else if ((key == "-wm") || (key == "--write-mode"))
					{
						parse_command_line_mode = GORBITSA::EParseCommandLineMode::WriteMode;
					}
					else if (std::filesystem::exists(argument))
					{
						file_paths.push_back(argument);
					}
					else
					{
						success = false;
					}
				}
				else if (std::filesystem::exists(argument))
				{
					file_paths.push_back(argument);
				}
				else
				{
					success = false;
				}
				break;
			case GORBITSA::EParseCommandLineMode::ProgramMemoryMode:
				key = argument;
				std::transform(key.begin(), key.end(), key.begin(), tolower);
				if (key == "rom")
				{
					program_memory_mode = GORBITSA::EProgramMemoryMode::ROM;
				}
				else if (key == "ram")
				{
					program_memory_mode = GORBITSA::EProgramMemoryMode::RAM;
				}
				else
				{
					success = false;
				}
				parse_command_line_mode = GORBITSA::EParseCommandLineMode::Nothing;
				break;
			case GORBITSA::EParseCommandLineMode::ReadMode:
				key = argument;
				std::transform(key.begin(), key.end(), key.begin(), tolower);
				if (key == "ascii")
				{
					read_mode = GORBITSA::EReadWriteMode::ASCII;
				}
				else if (key == "number")
				{
					read_mode = GORBITSA::EReadWriteMode::Number;
				}
				else
				{
					success = false;
				}
				parse_command_line_mode = GORBITSA::EParseCommandLineMode::Nothing;
				break;
			case GORBITSA::EParseCommandLineMode::WriteMode:
				key = argument;
				std::transform(key.begin(), key.end(), key.begin(), tolower);
				if (key == "ascii")
				{
					write_mode = GORBITSA::EReadWriteMode::ASCII;
				}
				else if (key == "number")
				{
					write_mode = GORBITSA::EReadWriteMode::Number;
				}
				else
				{
					success = false;
				}
				parse_command_line_mode = GORBITSA::EParseCommandLineMode::Nothing;
				break;
			}
		}
		if (!success)
		{
			break;
		}
	}
	if (success && (file_paths.size() > 0))
	{
		GORBITSA::VM vm(program_memory_mode);
		for (const std::string& file_path : file_paths)
		{
			std::ifstream input_file_stream(file_path);
			if (input_file_stream.is_open())
			{
				std::string input;
				std::string temp;
				while (std::getline(input_file_stream, temp))
				{
					input += '\n';
					input += temp;
				}
				if (vm.Load(input))
				{
					vm.Execute(read_mode, write_mode);
					vm.Clear();
				}
			}
		}
	}
	else
	{
		std::cout << "GORBITSA language interpreter" << std::endl <<
			std::endl <<
			"Parameters:" << std::endl <<
			"\t\"-pmm\" or \"--program-memory-mode\" <ROM|RAM>" << std::endl <<
			"\t\"-rm\" or \"--read-mode\" <ASCII|NUMBER>" << std::endl <<
			"\t\"-wm\" or \"--write-mode\" <ASCII|NUMBER>" << std::endl <<
			"\t<GORBITSA code file paths>" << std::endl <<
			std::endl;
		ret = 1;
	}
	return ret;
}
