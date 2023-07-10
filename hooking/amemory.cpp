/*
 * -----------------------------------------------------------------------------
 * Internal Memory Utility by Adafcaefc v.2021.1.1
 * -----------------------------------------------------------------------------
 *
 * This project is used in Hackermode, LotusInternal, and many other projects made by Adafcaefc
 * Special thanks to Andre for helping me
 *
 */
#include "amemory.h"
#include <climits>

namespace amemory
{
	bool write_bytes_protect(
		const std::uintptr_t address,
		const std::vector<std::uint8_t>& bytes)
	{
		DWORD oldProtect, newProtect;

		if (VirtualProtect(
			(LPVOID)address, bytes.size(),
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			if (write_bytes(address, bytes))
			{
				return VirtualProtect((LPVOID)address, bytes.size(), oldProtect, &newProtect);
			}
		}

		return false;
	}

	bool write_bytes_protect_with_float(
		const std::uintptr_t address,
		const float bytes)
	{
		DWORD oldProtect, newProtect;

		if (VirtualProtect(
			(LPVOID)address, 4,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			if (write_bytes_with_float(address, bytes))
			{
				return VirtualProtect((LPVOID)address, 4, oldProtect, &newProtect);
			}
		}

		return false;
	}

	bool write_variables_reference(
		const std::uintptr_t address,
		const float* bytes)
	{
		DWORD oldProtect, newProtect;

		if (VirtualProtect(
			(LPVOID)address, 4,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			if (write_bytes_with_float_reference(address, bytes))
			{
				return VirtualProtect((LPVOID)address, 4, oldProtect, &newProtect);
			}
		}

		return false;
	}

	bool write_bytes_protect_elegir(
		const std::uintptr_t address,
		const std::uintptr_t arraybytes)
	{
		DWORD oldProtect, newProtect;

		if (VirtualProtect(
			(LPVOID)address, 1,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			if (write_bytes_elegir(address, arraybytes))
			{
				return VirtualProtect((LPVOID)address, 1, oldProtect, &newProtect);
			}
		}

		return false;
	}

	std::uintptr_t load_pointer(
		const std::uintptr_t base,
		const std::vector<std::uintptr_t>& offsets)
	{
		DWORD oldProtect, newProtect;
		std::uint32_t address = base;

		for (auto i = 0; i < offsets.size(); ++i)
		{
			if (VirtualProtect(
				reinterpret_cast<LPVOID>(address),
				sizeof(address),
				PAGE_EXECUTE_READWRITE,
				&oldProtect))
			{
				VirtualProtect(
					reinterpret_cast<LPVOID>(address),
					sizeof(address),
					oldProtect,
					&newProtect);
			}
			else
			{
				return NULL;
			}
			address = read<std::uint32_t>(address);
			address += offsets.at(i);
		}

		return address;
	}

	bool write_bytes(
		const std::uintptr_t address,
		const std::vector<std::uint8_t>& bytes)
	{
		return WriteProcessMemory(
			GetCurrentProcess(),
			reinterpret_cast<LPVOID>(address),
			bytes.data(),
			bytes.size(),
			nullptr);
	}
	bool write_bytes_with_float(
		const std::uintptr_t address,
		const float bytes)
	{
		return WriteProcessMemory(
			GetCurrentProcess(),
			reinterpret_cast<LPVOID>(address),
			(LPVOID)&bytes,
			4,
			nullptr);
	}

	bool write_bytes_with_float_reference(
		const std::uintptr_t address,
		const float* bytes)
	{
		return WriteProcessMemory(
			GetCurrentProcess(),
			reinterpret_cast<LPVOID>(address),
			(LPVOID)&bytes,
			4,
			nullptr);
	}

	bool write_byteswithdword(
		const std::uintptr_t address,
		const DWORD bytes)
	{
		return WriteProcessMemory(
			GetCurrentProcess(),
			reinterpret_cast<LPVOID>(address),
			&bytes,
			4,
			nullptr);
	}

	bool write_byteschar(
		const std::uintptr_t address,
		const char* bytes)
	{
		return WriteProcessMemory(
			GetCurrentProcess(),
			reinterpret_cast<LPVOID>(address),
			&bytes,
			4,
			nullptr);
	}

	bool write_bytes_elegir(
		const std::uintptr_t address,
		const std::uintptr_t arraybytes)
	{
		return WriteProcessMemory(
			GetCurrentProcess(),
			reinterpret_cast<LPVOID>(address),
			&arraybytes,
			1,
			nullptr);
	}

	bool place_jump(
		const std::uintptr_t address,
		const std::uintptr_t destination_address,
		const size_t length)
	{
		DWORD oldProtect, newProtect;
		const std::uint32_t relativeAddress = destination_address - address - 5;

		if (!VirtualProtect(
			reinterpret_cast<LPVOID>(address),
			length,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			return false;
		}

		write<std::uint8_t>(address, 0xE9);
		write<std::uint32_t>(address + 1, relativeAddress);

		for (auto i = 5; i < length; ++i)
		{
			write<std::uint8_t>(address + i, 0x90);
		}

		return VirtualProtect(
			reinterpret_cast<LPVOID>(address),
			length,
			oldProtect,
			&newProtect);
	}

	std::uintptr_t get_base()
	{
		return reinterpret_cast<std::uintptr_t>(GetModuleHandle(NULL));
	}

	std::vector<std::uint8_t> read_bytes(
		const std::uintptr_t address,
		const size_t length)
	{
		std::vector<std::uint8_t> buffer;
		for (auto i = 0; i < length; ++i)
		{
			buffer.push_back(
				read<std::uint8_t>(
					address + i));
		}
		return buffer;
	}

	std::vector<std::uint8_t> read_bytes_protect(
		const std::uintptr_t address,
		const size_t length)
	{
		std::vector<std::uint8_t> empty_vector;

		DWORD oldProtect, newProtect;
		if (!VirtualProtect(
			reinterpret_cast<LPVOID>(address),
			length,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			return empty_vector;
		}

		auto buffer = read_bytes(address, length);

		VirtualProtect(
			reinterpret_cast<LPVOID>(address),
			length,
			oldProtect,
			&newProtect);

		return buffer;
	}

	bool push_write(
		const std::uintptr_t address,
		const std::uintptr_t destination_address,
		const size_t length)
	{
		DWORD oldProtect, newProtect;
		const std::uint32_t relativeAddress = destination_address - address - 5;

		if (!VirtualProtect(
			reinterpret_cast<LPVOID>(address),
			length,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			return false;
		}

		write<std::uint8_t>(address, 0xE9);
		write<std::uint32_t>(address + 1, relativeAddress);

		for (auto i = 5; i < length; ++i)
		{
			write<std::uint8_t>(address + i, 0x90);
		}

		return VirtualProtect(
			reinterpret_cast<LPVOID>(address),
			length,
			oldProtect,
			&newProtect);
	}

	bool push_write(
		const std::uintptr_t address,
		const DWORD destination_address)
	{
		DWORD oldProtect, newProtect;

		if (VirtualProtect(
			(LPVOID)address, 4,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			if (write_byteswithdword(address, destination_address))
			{
				return VirtualProtect((LPVOID)address, 4, oldProtect, &newProtect);
			}
		}

		return false;
	}

	bool push_write_with_char(
		const std::uintptr_t address,
		const char* destination_address)
	{
		DWORD oldProtect, newProtect;

		if (VirtualProtect(
			(LPVOID)address, 4,
			PAGE_EXECUTE_READWRITE,
			&oldProtect))
		{
			if (write_byteschar(address, destination_address))
			{
				return VirtualProtect((LPVOID)address, 4, oldProtect, &newProtect);
			}
		}

		return false;
	}

	bool pointer_write(const std::uintptr_t address, std::vector<DWORD> offsets, const std::vector<std::uint8_t>& bytes) {


		DWORD direccion_base = NULL;
		std::vector<DWORD> point_para_offset{ offsets };
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(get_base() + address), &direccion_base, sizeof(direccion_base), NULL);
		DWORD pointsAddress = direccion_base;
		for (int i = 0; i < point_para_offset.size() - 1; i++)
		{
			ReadProcessMemory(GetCurrentProcess(), (LPVOID)(pointsAddress + point_para_offset.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
		}
		pointsAddress += point_para_offset.at(point_para_offset.size() - 1);
		return WriteProcessMemory(GetCurrentProcess(), (LPVOID)pointsAddress, bytes.data(), bytes.size(), NULL);
	}

	bool pointer_write_int(const std::uintptr_t address, std::vector<DWORD> offsets, LPVOID bytes) {


		DWORD direccion_base = NULL;
		std::vector<DWORD> point_para_offset{ offsets };
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(get_base() + address), &direccion_base, sizeof(direccion_base), NULL);
		DWORD pointsAddress = direccion_base;
		for (int i = 0; i < point_para_offset.size() - 1; i++)
		{
			ReadProcessMemory(GetCurrentProcess(), (LPVOID)(pointsAddress + point_para_offset.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
		}
		pointsAddress += point_para_offset.at(point_para_offset.size() - 1);
		return WriteProcessMemory(GetCurrentProcess(), (LPVOID)pointsAddress, bytes, 4, NULL);
	}

	bool pointer_read(const std::uintptr_t address, std::vector<DWORD> offsets, int leer_bytes_asign) {


		DWORD direccion_base = NULL;
		std::vector<DWORD> point_para_offset{ offsets };
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(address), &direccion_base, sizeof(direccion_base), NULL);
		DWORD pointsAddress = direccion_base;
		for (int i = 0; i < point_para_offset.size() - 1; i++)
		{
			ReadProcessMemory(GetCurrentProcess(), (LPVOID)(pointsAddress + point_para_offset.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
		}
		pointsAddress += point_para_offset.at(point_para_offset.size() - 1);
		return ReadProcessMemory(GetCurrentProcess(), (LPVOID)(pointsAddress), &leer_bytes_asign, 4, 0);
	}

}