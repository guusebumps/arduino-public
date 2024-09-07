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
#include <Mitov_SoftwareSerial.h>
#include <Mitov_Maiyout_Serial_MP3.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//---------------------------------------------------------------------------
namespace PinCalls
{
	class T4DDuinoESP8266SerialPinCallerReceive
	{
	public:
		void Notify( void *_Data );

	};
} // PinCalls
//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	typedef Mitov::VisuinoSoftwareSerial<
	    Mitov::ConstantProperty<20, uint32_t, 9 >, // 0_ControlOutputPins0
		Mitov::ConstantProperty<21, uint32_t, 8 >, // 0_ControlOutputPins1
//		9, // 0
//		8, // 1
		Mitov::ConstantProperty<9, uint32_t, 0 >, // AfterSendingDelay
        Mitov::EmbeddedCallChain<SoftwareSerial_CallChains::GetPeriod >, // Elements_GetPeriod
		Mitov::ConstantProperty<7, bool, true >, // Enabled
		Mitov::ConstantProperty<16, uint32_t, 0 >, // FElementIndex = 0
		Mitov::ConstantProperty<27, uint32_t, 0 >, // FEndTime = 0
		Mitov::ConstantProperty<23, bool, false >, // FInBreak = False
		Mitov::ConstantProperty<25, bool, false >, // FSending = False
		Mitov::GenericPin_EmbeddedPinImplementation<5, ::PinCalls::T4DDuinoESP8266SerialPinCallerReceive>, // OutputPin
		Mitov::NestedProperty<15, Mitov::TArduinoSerialBreak<
		  Mitov::ConstantProperty<10, bool, false >, // Enabled = False (Default)
		  Mitov::NestedProperty<14, Mitov::TAurduinoEnableValue<
			Mitov::ConstantProperty<12, bool, false >, // Enabled = False (Default)
			Mitov::ConstantProperty<13, uint32_t, 100 > // Value = 100 (Default)
			 > > // Period
		  > >, // SendBreak
		Mitov::DigitalPin_NoImplementation<4 >, // SendingOutputPin
		Mitov::ConstantProperty<8, uint32_t, 19200 > // Speed
	> T4DDuinoESP8266Serial;
//---------------------------------------------------------------------------
	T4DDuinoESP8266Serial GESP8266SoftwareSerial;
//---------------------------------------------------------------------------
	class T4DDuinoESP8266Reset
	{
	public:
		template <typename T> static inline void Reset( T *AInstance )
		{
			digitalWrite( 17, HIGH );
			delay(100);
			digitalWrite( 17, LOW );
			delay(3000);
			
			GESP8266SoftwareSerial.SetSpeed( 115200 );
			delay(100);
			GESP8266SoftwareSerial.GetStream().println( "AT+UART_CUR=19200,8,1,0,0" );
			delay(10);
			GESP8266SoftwareSerial.SetSpeed( 19200 );
			delay(10);
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

