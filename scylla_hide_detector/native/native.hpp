#pragma once

namespace native {
	int strlen(const char* str);
	int strlen(wchar_t* str);
	bool strcmp(char* first_string, char* second_string);
	bool strcmp(wchar_t* first_string, wchar_t* second_string);
	void* get_dll_base(const wchar_t* dllname);
	void* find_export(const wchar_t* module_name, const char* export_name);
}
