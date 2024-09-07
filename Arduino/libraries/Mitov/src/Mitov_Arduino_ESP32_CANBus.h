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
#include <driver/twai.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_FSending,
		typename T_FailedOutputPin,
		typename T_SentOutputPin
	> class TArduinoCanBusBasicSendMessage_ESP32_Element :
		public T_FSending,
		public T_FailedOutputPin,
		public T_SentOutputPin
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SentOutputPin )

	protected:
		_V_PROP_( FSending )
		volatile bool FSent = false;
		volatile bool FFailed = false;

	protected:
		inline void WaitForSending()
		{
			while( FSending() )
			{
				delay( 1 );
				if( FSent || FFailed )
					break;

			}

			FSending() = true;
		}

	public:
		inline void MessageSendCompeled( bool ASuccess )
		{
			if( ASuccess )
				FSent = true;

			else
				FFailed = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FSent )
			{
				FSent = false;
				T_SentOutputPin::ClockPin();
			}

			if( FFailed )
			{
				FFailed = false;
				T_FailedOutputPin::ClockPin();
			}

			FSending() = false;
		}

	public:
		inline TArduinoCanBusBasicSendMessage_ESP32_Element()
		{
			FSending() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
		typename T_FSending,
		typename T_FailedOutputPin,
		typename T_ID,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_Request,
		typename T_Retry,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class TArduinoCanBusSendMessage_ESP32_Element :
		public TArduinoCanBusBasicSendMessage_ESP32_Element<
				T_FSending,
				T_FailedOutputPin,
				T_SentOutputPin
			>,
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_ID,
		public T_InitialValue,
		public T_Request,
		public T_Retry,
		public T_SendTimeout
	{
		typedef TArduinoCanBusBasicSendMessage_ESP32_Element<
				T_FSending,
				T_FailedOutputPin,
				T_SentOutputPin
			> inherited;

	public:
		_V_PROP_( Extended )
		_V_PROP_( ID )
		_V_PROP_( InitialValue )
		_V_PROP_( Request )
		_V_PROP_( Retry )
		_V_PROP_( SendTimeout )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			inherited::WaitForSending();

            const uint8_t *ABuffer = T_InitialValue::Allocate();
   			if( ! C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), Request().GetValue(), T_InitialValue::GetCount(), ABuffer, Retry(), C_INDEX, SendTimeout() ))
				T_FailedOutputPin::ClockPin();

            T_InitialValue::Release( ABuffer );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
            InitialValue() = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
		typename T_ExtendedOutputPin,
		typename T_FSending,
		typename T_FailedOutputPin,
		typename T_ID,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_MessageOutputPin,
		typename T_RequestOutputPin,
		typename T_Retry,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class Microchip_ESP32_CanBus_RequestMessage :
		public TArduinoCanBusBasicSendMessage_ESP32_Element<
				T_FSending,
				T_FailedOutputPin,
				T_SentOutputPin
			>,
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_ExtendedOutputPin,
		public T_ID,
		public T_InitialValue,
		public T_MessageOutputPin,
		public T_RequestOutputPin,
		public T_Retry,
		public T_SendTimeout
	{
		typedef TArduinoCanBusBasicSendMessage_ESP32_Element<
				T_FSending,
				T_FailedOutputPin,
				T_SentOutputPin
			> inherited;

	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( Extended )
		_V_PROP_( ID )
		_V_PROP_( InitialValue )
		_V_PROP_( Retry )
		_V_PROP_( SendTimeout )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
//			USBSerial.println( "TEST1" );
//			USBSerial.println( ID().GetValue() );
			inherited::WaitForSending();

            const uint8_t *ABuffer = T_InitialValue::Allocate();
   			if( ! C_OWNER.SendMessage( ID().GetValue(), Extended().GetValue(), true, T_InitialValue::GetCount(), ABuffer, Retry(), C_INDEX, SendTimeout() ))
				T_FailedOutputPin::ClockPin();

            T_InitialValue::Release( ABuffer );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
            InitialValue() = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );

		}

	public:
		inline void ProcessMessage( uint32_t AID, bool AExtended, bool ARequest, uint8_t  ALength, const uint8_t * ABuffer )
		{
			if( AID != ID().GetValue() )
				return;

//			T_IDOutputPin::SetPinValue( *(uint32_t *)ABuffer );

			T_ExtendedOutputPin::SetPinValue( AExtended );
			T_RequestOutputPin::SetPinValue( ARequest );

			if( T_MessageOutputPin::GetPinIsConnected() )
			{
				TDataBlock AData( ALength, ABuffer );
				T_MessageOutputPin::SetPinValue( AData );
			}

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Clock,
		typename T_RX,
		typename T_TX
	> class TArduinoESP32CanBusModulePins :
		public T_Clock,
		public T_RX,
		public T_TX
	{
	public:
		_V_PROP_( Clock )
		_V_PROP_( RX )
		_V_PROP_( TX )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TArduinoESP32CanBusModuleQueueCount :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadValue()
		{
			twai_status_info_t AInfo;
			if( twai_get_status_info( &AInfo ) != ESP_OK )
			{
				T_OutputPin::SetPinValue( 0 );
				return;
			}

			T_OutputPin::SetPinValue( AInfo.msgs_to_rx );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue();
		}

	public:
		inline void SystemStart()
		{
			ReadValue();
		}

		inline void SystemLoopBegin()
		{
			ReadValue();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class TArduinoESP32CanBusModuleTransmitQueueCount :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void ReadValue()
		{
			twai_status_info_t AInfo;
			if( twai_get_status_info( &AInfo ) != ESP_OK )
			{
				T_OutputPin::SetPinValue( 0 );
				return;
			}

			T_OutputPin::SetPinValue( AInfo.msgs_to_tx );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue();
		}

	public:
		inline void SystemStart()
		{
			ReadValue();
		}

		inline void SystemLoopBegin()
		{
			ReadValue();
		}

	};
//---------------------------------------------------------------------------
	template <
//		typename T_Count,
		typename T_Size
	> class TArduinoESP32CanBusModuleQueue :
//		public T_Count,
		public T_Size
	{
	public:
//		_V_PROP_( Count )
		_V_PROP_( Size )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Receive,
		typename T_Transmit
	> class TArduinoESP32CanBusModuleQueues :
		public T_Receive,
		public T_Transmit
	{
	public:
		_V_PROP_( Receive )
		_V_PROP_( Transmit )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Retry,
		typename T_Timeout
	> class TArduinoESP32CanBus_BasicModule_Sending :
		public T_Retry,
		public T_Timeout
	{
	public:
		_V_PROP_( Retry )
		_V_PROP_( Timeout )

	};
//---------------------------------------------------------------------------
	template <
		typename T_BusSpeed,
		typename T_Elements_MessageSendCompeled,
		typename T_Elements_ProcessMessage,
		typename T_Enabled,
		typename T_ExtendedOutputPin,
		typename T_IDOutputPin,
		typename T_INSTANCE_DoTransmit,
		typename T_ListenOnly,
		typename T_MessageOutputPin,
		typename T_OutputPin,
		typename T_Pins,
		typename T_Queues,
		typename T_RequestOutputPin,
		typename T_Sending,
		typename T_Testing
	> class TArduino_ESP32_CANBus_Module :
		public T_BusSpeed,
		public T_Enabled,
		public T_ExtendedOutputPin,
		public T_IDOutputPin,
		public T_ListenOnly,
		public T_MessageOutputPin,
		public T_OutputPin,
		public T_Pins,
		public T_Queues,
		public T_RequestOutputPin,
		public T_Sending,
		public T_Testing
	{
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( IDOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( OutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( BusSpeed )
		_V_PROP_( Enabled )
		_V_PROP_( ListenOnly )
		_V_PROP_( Pins )
		_V_PROP_( Queues )
		_V_PROP_( Sending )
		_V_PROP_( Testing )

	protected:
		struct TSendItem
		{
			uint8_t SenderIndex;
			bool IsRequest : 1;
			bool IsExtended : 1;
			bool Retry : 1;
			uint8_t Size : 6;
			uint32_t Timeout;

			uint32_t ID;
			uint8_t Buffer[ 8 ];
		};

	public:
		void DoTransmit()
		{
            while( true )
            {
				TSendItem AItem;
                xQueueReceive( FTXQueue, &AItem, portMAX_DELAY );
				SendMessageDirect( AItem.ID, AItem.IsExtended, AItem.IsRequest, AItem.Size, AItem.Buffer, AItem.Retry, AItem.Timeout );
			}
		}

	protected:
        static void StaticDoTransmit(void *)
        {
            T_INSTANCE_DoTransmit::Call();
        }

	protected:
        TaskHandle_t    FTXTask = nullptr;
        QueueHandle_t   FTXQueue = nullptr;

	protected:
		void Initialize()
		{
			twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT( gpio_num_t( Pins().TX().GetValue() ), gpio_num_t( Pins().RX().GetValue() ), TWAI_MODE_LISTEN_ONLY );
			g_config.clkout_io = gpio_num_t( Pins().Clock().GetValue() );

//			g_config.tx_queue_len = Queues().Transmit().Size();
			g_config.tx_queue_len = 0;

			g_config.rx_queue_len = Queues().Receive().Size();
//			g_config.rx_queue_len = 0;

			if( Testing() )
				g_config.mode = TWAI_MODE_NO_ACK;

			else if( ListenOnly() )
				g_config.mode = TWAI_MODE_LISTEN_ONLY;

			else
				g_config.mode = TWAI_MODE_NORMAL;

			twai_timing_config_t t_config;

			if( BusSpeed() < 50 )
				t_config = TWAI_TIMING_CONFIG_25KBITS();

			else if( BusSpeed() < 100 )
				t_config = TWAI_TIMING_CONFIG_50KBITS();

			else if( BusSpeed() < 125 )
				t_config = TWAI_TIMING_CONFIG_100KBITS();

			else if( BusSpeed() < 250 )
				t_config = TWAI_TIMING_CONFIG_125KBITS();

			else if( BusSpeed() < 500 )
				t_config = TWAI_TIMING_CONFIG_250KBITS();

			else if( BusSpeed() < 800 )
				t_config = TWAI_TIMING_CONFIG_500KBITS();

			else if( BusSpeed() < 1000 )
				t_config = TWAI_TIMING_CONFIG_800KBITS();

			else
				t_config = TWAI_TIMING_CONFIG_1MBITS();

			twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

			twai_driver_install( &g_config, &t_config, &f_config );
		}

		bool SendMessageDirect( uint32_t id, bool AExtended, bool ARequest, uint8_t len, const uint8_t *buf, bool ARetry, uint32_t ATimeout )
		{
			twai_message_t AMessage;
			AMessage.extd = false;
			AMessage.rtr = ARequest;
			AMessage.ss = ! ARetry;
			AMessage.self = false;
			AMessage.dlc_non_comp = false;
			AMessage.reserved = 0;

			AMessage.identifier = id;
			AMessage.data_length_code = len;
			memcpy( AMessage.data, buf, MitovMin<uint8_t>( 8, len ));

//			USBSerial.println( "X" );
//			USBSerial.println( AMessage.identifier );
			delay( 1 );
			return ( twai_transmit( &AMessage, pdMS_TO_TICKS( ATimeout )) == ESP_OK );
		}

	public:
		inline void UpdateEnabled()
		{
			if( Enabled() )
			{
				if( Queues().Transmit().Size() )
				{
					FTXQueue = xQueueCreate( Queues().Transmit().Size(), sizeof( TSendItem ) );
					if( ! FTXTask )
						xTaskCreate(
							StaticDoTransmit,   // Function to implement the task
							"DoTransmit", // Name of the task
							20000,                   // Stack size in words
							NULL,                    // Task input parameter
							2,                       // Priority of the task
		//                    &FCallbackTaskInputHandle     // Task handle.
							&FTXTask     // Task handle.
						);

				}

				twai_start();
			}

			else
			{
				twai_stop();
				if( FTXQueue )
				{
//					vTaskDelete( FTXTask );
//					FTXTask = nullptr;

					vQueueDelete( FTXQueue );
					FTXQueue = nullptr;
				}
			}
			
		}

		inline void UpdateConfig()
		{
			twai_driver_uninstall();
			Initialize();
		}

	public:
		bool SendMessage( uint32_t id, bool AExtended, bool ARequest, uint8_t len, const uint8_t *buf, bool ARetry, uint8_t ASenderIndex, uint32_t ATimeout )
		{
			if( ! Enabled().GetValue() )
				return true;

			if( ! FTXQueue )
				return SendMessageDirect( id, AExtended, ARequest, len, buf, ARetry, ATimeout );

			TSendItem AItem;
			AItem.SenderIndex = ASenderIndex;
			AItem.IsRequest = ARequest;
			AItem.IsExtended = AExtended;
			AItem.ID = id;
			AItem.Size = len;
			AItem.Retry = ARetry;
			AItem.Timeout = ATimeout;

			memcpy( AItem.Buffer, buf, MitovMin<uint8_t>( sizeof( AItem.Buffer ), len ));

			xQueueSend( FTXQueue, &AItem, portMAX_DELAY );
/*
			while( xQueueSend( FTXQueue, &AItem, TickType_t( 10000 )) != pdTRUE )
			{
				delay( 1 );
				USBSerial.println( "TRY" ); // Wait forever
			}
*/
			return true;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TCanBusPacket &ADataBlock = *(Mitov::TCanBusPacket*)_Data;
			SendMessage( ADataBlock.GetID(), ADataBlock.IsExtended(), ADataBlock.IsRequest(), ADataBlock.GetSize(), ADataBlock.Read(), -1, Sending().Retry().GetValue(), Sending().Timeout().GetValue() );
		}

	public:
		inline void SystemInit()
		{
//			USBSerial.begin();
			Initialize();
			UpdateEnabled();
		}

		inline void SystemLoopBegin()
		{
			twai_message_t AMessage;
			if( twai_receive( &AMessage, 0 ) == ESP_OK )
			{
				T_Elements_ProcessMessage::Call( AMessage.identifier, AMessage.extd, AMessage.rtr, AMessage.data_length_code, AMessage.data );
				T_IDOutputPin::SetPinValue( AMessage.identifier );
				T_ExtendedOutputPin::SetPinValue( AMessage.extd );

				T_RequestOutputPin::SetPinValue( AMessage.rtr );

				if( T_MessageOutputPin::GetPinIsConnected() )
				{
					TDataBlock AData( AMessage.data_length_code, AMessage.data );
					T_MessageOutputPin::SetPinValue( AData );
				}

				if( T_OutputPin::GetPinIsConnected() )
				{
					Mitov::TCanBusPacket AData( AMessage.identifier, AMessage.extd, AMessage.rtr, AMessage.data_length_code, AMessage.data );
					T_OutputPin::SetPinValue( AData );
				}
			}
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


