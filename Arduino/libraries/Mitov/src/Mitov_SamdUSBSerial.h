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
#include <Mitov_StandardSerial.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_SendingDelay
	> class SamdUSBSerialDelayImplementation : 
		public USBSerialDelayImplementation<
				T_SERIAL_TYPE, C_SERIAL, 
				115200
			>,
		public T_SendingDelay
	{
	public:
		_V_PROP_( SendingDelay )

	public:
		inline size_t SendingWrite( uint8_t *AData, uint32_t ASize )
		{
			uint32_t ASendSize = ASize;
			while( ASendSize )
			{
				uint32_t ASubSize = MitovMin<uint32_t>( C_SERIAL.availableForWrite(), ASendSize );
   				uint32_t AResult = C_SERIAL.write( AData, ASubSize );

                ASendSize -= AResult;
                AData += AResult;
                delay( SendingDelay() );
			}

			return ASendSize;
		}

		inline void UpdateSending( bool ASending ) {}

	};
//---------------------------------------------------------------------------
	template <
		typename T_SERIAL_TYPE, T_SERIAL_TYPE &C_SERIAL,
		typename T_AfterSendingDelay,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_SendingDelay,
		typename T_SendingOutputPin,
		typename T_WaitForConnection
	> class SamdUSBPort :  // ESP32_USBSerialPort
		public BasicSendingSerialPort <
			T_SERIAL_TYPE, C_SERIAL,
			ConnectionUSBSerialPortImplementation<
					T_SERIAL_TYPE, C_SERIAL,
					SamdUSBSerialDelayImplementation <
							T_SERIAL_TYPE, C_SERIAL,
							T_SendingDelay
						>,
					T_ConnectedOutputPin,
					T_WaitForConnection
				>,
				T_AfterSendingDelay,
				T_Enabled,
				T_FEndTime,
				T_FSending,
				T_OutputPin,
				T_SendingOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
} // Mitov



