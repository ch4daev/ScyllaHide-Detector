#include "scylla_detector.hpp"

int main()
{
	if (scylla::is_attached()) {
		//scylla hide detected
		//exiting...
	}
	else {
		//scylla hide not detected, ok!
	}
}


