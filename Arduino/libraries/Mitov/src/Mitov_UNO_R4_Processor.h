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

//---------------------------------------------------------------------------
extern "C" char* sbrk(int incr);
//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled
	> class UNO_R4_ProcessorReset :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				NVIC_SystemReset();

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TArduino_UNO_R4_ProcessorAvailableRAMOperation : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		int freeMemory()
		{
			char top;
			return &top - reinterpret_cast<char*>(sbrk(0));
		}

		void ReadValue( bool AChangeOnly )
		{
			if( ! Enabled().GetValue() )
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