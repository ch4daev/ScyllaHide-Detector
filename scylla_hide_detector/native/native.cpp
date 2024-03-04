
/*
* @brief : All methods in this file in one way or another implement functions 
*		   similar to the original functions from winapi
* 
* For the most part the code is not elaborate and does not check the data that is fed into the input.I wrote it with the expectation of personal use to be able to get rid of the use of all imports
*/

#include "native.hpp"

#include "windows_structs.hpp"


extern "C" _PEB * get_peb();

namespace native {
	int strlen(const char *str) {
		int i;

		for (i = 0; str[i] != 0x0 ; i++) {
			
		}

		return i;
	}
	int strlen(wchar_t* str) {
		int i;

		for (i = 0; str[i] != 0x0; i+=2)
		{
			
		}
		return i;
	}

	bool strcmp(char* first_string,char* second_string) {

		int first_string_size = strlen(first_string);
		int second_string_size = strlen(second_string);

		if (first_string_size != second_string_size) {
			return 0;
		}

		for (int i = 0; i < first_string_size; i++)
		{
			if (first_string[i] != second_string[i]) {
				return 0;
			}
		}

		return 1;
	}
	bool strcmp(wchar_t* first_string, wchar_t* second_string) {
		int first_string_size = strlen(first_string);
		int second_string_size = strlen(second_string);

		if (first_string_size != second_string_size) {
			return 0;
		}

		for (int i = 0; i < first_string_size; i+=2)
		{
			if (first_string[i] != second_string[i]) {
				return 0;
			}
		}

		return 1;
	}
	/*
	* @brief : finding module base in peb->Ldr->InMemoryOrderModuleList by name
	*
	* GetModuleHandle implementation
	* 
	* @param : module name in const wchar_t * type
	* @return : pointer to base module address, if module not found nullptr
	*/
	void* get_dll_base(const wchar_t* dllname) {
		_PEB* peb = get_peb();
		_LDR_DATA_ITEM* ldr_table_item = reinterpret_cast<_LDR_DATA_ITEM*>(peb->Ldr->InMemoryOrderModuleList.Flink);
		_LDR_DATA_ITEM* ldr_table_end = reinterpret_cast<_LDR_DATA_ITEM*>(peb->Ldr->InMemoryOrderModuleList.Blink);

		do
		{

			if (strcmp(ldr_table_item->mod_name, const_cast<wchar_t*>(dllname))) {

				return ldr_table_item->base;
			}

			ldr_table_item = reinterpret_cast<_LDR_DATA_ITEM*>(ldr_table_item->list.Flink);
		} while (ldr_table_item != ldr_table_end);

		return nullptr;
	}

	/*
	* @brief : finding export function address from module by name
	*
	* @param1 : module name in const wchar *type
	* @param2 : function name in const char  *type
	* 
	* GetProcAddress implementation
	*
	* @return : function address, if not found nullptr
	*/
	void* find_export(const wchar_t* module_name, const char* export_name) {
		void* module_base = get_dll_base(module_name);

		if (module_base == nullptr) {

			return nullptr;
		}

		_IMAGE_DOS_HEADER* module_dos = reinterpret_cast<_IMAGE_DOS_HEADER*>(module_base);
		_IMAGE_NT_HEADERS64* module_nt = reinterpret_cast<_IMAGE_NT_HEADERS64*>(module_dos->e_lfanew + reinterpret_cast<char*>(module_base));

		_IMAGE_EXPORT_DIRECTORY* ntdll_export_directory = reinterpret_cast<_IMAGE_EXPORT_DIRECTORY*>(module_nt->OptionalHeader.DataDirectory[0].VirtualAddress + reinterpret_cast<char*>(module_base));

		int* function_addreses_rva = reinterpret_cast<int*>(ntdll_export_directory->AddressOfFunctions + reinterpret_cast<char*>(module_base));
		int* function_names_rva = reinterpret_cast<int*>(ntdll_export_directory->AddressOfNames + reinterpret_cast<char*>(module_base));
		short* function_ordinal_rva = reinterpret_cast<short*>(ntdll_export_directory->AddressOfNameOrdinals + reinterpret_cast<char*>(module_base));


		for (int i = 0; i < ntdll_export_directory->NumberOfNames; i++)
		{
			char* function_name = reinterpret_cast<char*>(function_names_rva[i] + reinterpret_cast<char*>(module_dos));

			if (strcmp(function_name, const_cast<char*>(export_name))) {
				int* function_address = reinterpret_cast<int*>(function_addreses_rva[function_ordinal_rva[i]] + reinterpret_cast<char*>(module_dos));

				return function_address;
			}
		}

		return nullptr;
	}
}
