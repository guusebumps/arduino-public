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
#ifdef VISUINO_ELEGOO_PENGUIN_BOT_V2
namespace BoardDeclarations
{
namespace Types
{
class VisuinoSoftwareSerial_NullCallChain
{
public:
	static inline bool ChainHasItems() { return false; }
	inline static void Call( int32_t AIndex, unsigned long & APeriod ) {}

};
//---------------------------------------------------------------------------
typedef Mitov::VisuinoSoftwareSerial<
  17, // 0_ControlOutputPins0
  16, // 0_ControlOutputPins1
  Mitov::ConstantProperty<8, uint32_t, 0 >, // AfterSendingDelay = 0 (Default)
  VisuinoSoftwareSerial_NullCallChain, // Elements_GetPeriod
  Mitov::ConstantProperty<3, bool, true >, // Enabled = True (Default)
  Mitov::ConstantProperty<16, uint32_t, 0 >, // FElementIndex = 0
  Mitov::ConstantProperty<25, uint32_t, 0 >, // FEndTime = 0
  Mitov::ConstantProperty<21, bool, false >, // FInBreak = False
  Mitov::ConstantProperty<23, bool, false >, // FSending = False
  Mitov::GenericPin_NoImplementation<8 >, // OutputPin
  Mitov::NestedProperty<15, Mitov::TArduinoSerialBreak<
    Mitov::ConstantProperty<10, bool, false >, // Enabled = False (Default)
    Mitov::NestedProperty<14, Mitov::TAurduinoEnableValue<
      Mitov::ConstantProperty<12, bool, false >, // Enabled = False (Default)
      Mitov::ConstantProperty<13, uint32_t, 100 > // Value = 100 (Default)
       > > // Period
     > >, // SendBreak
  Mitov::DigitalPin_NoImplementation<6 >, // SendingOutputPin
  Mitov::ConstantProperty<7, uint32_t, 9600 > // Speed = 9600 (Default)
  > SerialPort1; // TArduino Software Serial
}

namespace Instances
{
	BoardDeclarations::Types::SerialPort1 SerialPort1;
}

}
#endif // VISUINO_ELEGOO_PENGUIN_BOT_V2
//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL, T_SERIAL &C_SERIAL,
		typename T_Element_Response,
		typename T_Elements_UpdateOutput,
		typename T_Enabled,
		typename T_EqualizerMode,
		typename T_ErrorOutputPin,
		typename T_FExpectFormatResult,
		typename T_FExpectReset,
		typename T_FFirstTry,
		typename T_FIsSending,
		typename T_Playback,
		typename T_SDCard,
		typename T_TimeOutPeriod,
		typename T_TimeoutErrorOutputPin,
		typename T_Track,
		typename T_Volume
	> class ElegooPenguinBotMP3Player :
		public MitovMaiyoutSerialMP3<
			T_SERIAL, C_SERIAL,
			T_Element_Response,
			T_Elements_UpdateOutput,
			T_Enabled,
			T_EqualizerMode,
			T_ErrorOutputPin,
			T_FExpectFormatResult,
			T_FExpectReset,
			T_FFirstTry,
			T_FIsSending,
			T_Playback,
			T_SDCard,
			T_TimeOutPeriod,
			T_TimeoutErrorOutputPin,
			T_Track,
			T_Volume
		>
	{
	public:
		typedef MitovMaiyoutSerialMP3<
				T_SERIAL, C_SERIAL,
				T_Element_Response,
				T_Elements_UpdateOutput,
				T_Enabled,
				T_EqualizerMode,
				T_ErrorOutputPin,
				T_FExpectFormatResult,
				T_FExpectReset,
				T_FFirstTry,
				T_FIsSending,
				T_Playback,
				T_SDCard,
				T_TimeOutPeriod,
				T_TimeoutErrorOutputPin,
				T_Track,
				T_Volume
			> inherited;

	public:
		inline void UpdateEnabled()
		{
			digitalWrite( 7, ( Enabled().GetValue() ) ? HIGH : LOW );
		}

//	protected:
//		static inherited	*FInstance;

	protected:
//		static void PinCallerReceive( void *_Data )
//		{
//			FInstance->_DirectPinReceive( _Data );
//		}

	public:
		inline void SystemInit( bool AEnabled )
		{
//			FInstance = this;
//			GSoftwareSerial.OutputPin().SetCallback( PinCallerReceive );

			pinMode( 7, OUTPUT );
			digitalWrite( 7, (AEnabled) ? HIGH : LOW );

			C_SERIAL.SystemInit();
			inherited::SystemInit();
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ELEGOO_PENGUIN_BOT_V2
			int AData = C_SERIAL.GetStream().read();
			if( AData >= 0 )
				inherited::ProcessChar( AData );
#else
			C_SERIAL.SystemLoopBegin();
#endif
			inherited::SystemLoopBegin();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif