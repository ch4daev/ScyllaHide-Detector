#include "scylla_detector.hpp"

#include "native/windows_structs.hpp"

#include "native/native.hpp"


namespace scylla {
	/*
	* @brief Checks if a given function has been hooked.
	*
	* This function examines a specified function to determine if it has been hooked.
	* It does this by looking for a jump instruction at the beginning of the function's code.
	* If such an instruction is found, it indicates that the function has been hooked.
	*
	* @param hooked_function A structure containing the module name and function name to check.
	*
	* @return Returns true if the function was hooked, false otherwise.
	*/
	bool function_is_hooked(const HookedFunction &hooked_function) {
		void* function_address = native::find_export(hooked_function.dllname, hooked_function.name);

		if (function_address == nullptr) {
			return false;
		}

		if (*reinterpret_cast<unsigned char*>(function_address) != 0x90) {
			return false;
		}

		return true;
	}

	/*
	* @brief Iterates over all functions that are intended to be hooked by Scylla Hide.
	*
	* This function checks each function in the provided list to determine if it has been hooked.
	* It uses the function_is_hooked() method to check each function individually.
	* If any function in the list has been hooked, the method returns true.
	*
	* @return Returns true if at least one function in the list has been hooked, false otherwise.
	*/
	bool is_attached() {
		bool status = false;

		for (const auto& function : hooked_functions)
		{
			status |= function_is_hooked(function);
		}

		return status;
	}
}
