////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_SPI_MicroSDCard.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_I2C, T_I2C &C_I2C,
		typename T_Enabled,
		typename T_ErrorInfoOutputPin,
		typename T_FRunning,
		typename T_FailedOutputPin,
		typename T_InsertedOutputPin,
		typename T_SuccessOutputPin,
		typename T_TYPE
	> class TArduinoM5StackCoreS3MicroSDModule : 
		public MicroSDCardCustom <
			SPIClass, SPI,
			Mitov::ConstantProperty<9, uint32_t, 4 >, // ChipSelectOutputPin
			T_Enabled,
			T_ErrorInfoOutputPin,
			T_FRunning,
			T_FailedOutputPin,
			T_SuccessOutputPin,
			T_TYPE
		>,
		public T_InsertedOutputPin
	{
	public:
		_V_PIN_( InsertedOutputPin )

#ifdef __M5STACK_DISPLAY__
	protected:
		typedef MicroSDCardCustom <
			SPIClass, SPI,
			Mitov::ConstantProperty<9, uint32_t, 4 >, // ChipSelectOutputPin
			T_Enabled,
			T_ErrorInfoOutputPin,
			T_FRunning,
			T_FailedOutputPin,
			T_SuccessOutputPin,
			T_TYPE
		> inherited;

	protected:
//		uint8_t	FLockCount = 0;

#endif // __M5STACK_DISPLAY__
	public:
		inline void BeginOperation()
		{
#ifdef __M5STACK_DISPLAY__
//			if( FLockCount ++ )
//				return;

//			delay( 10 );
			if( CoreS3_Pin35Mode::Output )
			{
				pinMode( 35, INPUT );
				CoreS3_Pin35Mode::Output = false;
			}
#endif // __M5STACK_DISPLAY__
		}

		inline void EndOperation()
		{
#ifdef __M5STACK_DISPLAY__
//			if( -- FLockCount )
//				return;

//			delay( 10 );
//			pinMode( 35, OUTPUT );
//			digitalWrite( 35, HIGH );
//			delay( 10 );
#endif // __M5STACK_DISPLAY__
		}

#ifdef __M5STACK_DISPLAY__
	public:
		bool DoExists( Mitov::String APathName )
		{
			BeginOperation();
			bool AResult = inherited::DoExists( APathName );
			EndOperation();
			return AResult;
		}

		bool DoCreate( Mitov::String APathName )
		{
			BeginOperation();
			bool AResult = inherited::DoCreate( APathName );
			EndOperation();
			return AResult;
		}

		bool DoRemoveDir( Mitov::String APathName )
		{
			BeginOperation();
			bool AResult = inherited::DoRemoveDir( APathName );
			EndOperation();
			return AResult;
		}

		bool DoRemoveFile( Mitov::String APathName )
		{
			BeginOperation();
			bool AResult = inherited::DoRemoveFile( APathName );
			EndOperation();
			return AResult;
		}

		bool DoOpen( Mitov::String APathName, MITOV_FILE_MODE mode, File &AFile )
		{
			BeginOperation();
			bool AResult = inherited::DoOpen( APathName, mode, AFile );
			EndOperation();
			return AResult;
		}

	public:
		void UpdateEnabled()
		{
			BeginOperation();
			inherited::UpdateEnabled();
			EndOperation();
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

#endif // __M5STACK_DISPLAY__
	public:
		inline void SystemStartInserted()
		{
			C_I2C.beginTransmission( uint8_t( 0x58 ));
			C_I2C.write( 0x00 );
			C_I2C.endTransmission();
			C_I2C.requestFrom( uint8_t( 0x58 ), _VISUINO_I2C_SIZE_( 1 ));
			uint8_t AValue = C_I2C.read();
			T_InsertedOutputPin::SetPinValue( ( AValue & 0b10000 ) == 0 );
		}

		inline void SystemLoopBeginInserted()
		{
			SystemStartInserted();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif
