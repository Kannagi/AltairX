#ifndef EXECUTION_HPP_INCLUDED
#define EXECUTION_HPP_INCLUDED

#include "vm_wrapper.hpp"

inline void execute(ar::processor& processor)
{
	while(true)
	{
		processor.decode();

		if(!processor.execute())
		{
			break;
		}

		processor.direct_memory_access();
	}
}

#endif
