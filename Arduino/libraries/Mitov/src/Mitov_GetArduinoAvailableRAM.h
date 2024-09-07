////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

extern unsigned int __heap_start;
extern void *__brkval;

// The free list structure as maintained by the 
// avr-libc memory allocation routines.

namespace Mitov
{
	struct __freelist {
	  size_t sz;
	  struct __freelist *nx;
	};
}

// The head of the free list structure
extern struct Mitov::__freelist *__flp;

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class GetArduinoAvailableRAM : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		// Calculates the size of the free list
		int freeListSize() 
		{
			struct __freelist* current;
			int total = 0;
			for (current = __flp; current; current = current->nx) 
			{
				total += 2; /* Add two bytes for the memory block's header  */
				total += (int) current->sz;
			}

			return total;
		}

		int freeMemory()
		{
			int free_memory;
			if ((int)__brkval == 0) 
				free_memory = ((int)&free_memory) - ((int)&__heap_start);

			else 
			{
				free_memory = ((int)&free_memory) - ((int)__brkval);
				free_memory += freeListSize();
			}

			return free_memory;
		}

		void ReadValue( bool AChangeOnly )
		{
			if( ! Enabled() )
				return;

			T_OutputPin::SetPinValue( freeMemory(), AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue( false );
		}

	public:
		inline void SystemStart()
		{
			ReadValue( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValue( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
