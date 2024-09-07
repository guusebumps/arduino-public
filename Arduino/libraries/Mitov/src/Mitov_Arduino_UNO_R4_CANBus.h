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
#include <r_can.h>
#include <sync.h>
#include <CanUtil.h>
#include <IRQManager.h>
#include <Mitov_CanBus.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace UNO_R4_CANBus
	{
		  uint32_t constexpr CAN_EFF_FLAG    = 0x80000000U;
		  uint32_t constexpr CAN_SFF_MASK    = 0x000007FFU; /* standard frame format (SFF) */
		  uint32_t constexpr CAN_EFF_MASK    = 0x1FFFFFFFU; /* extended frame format (EFF) */
//---------------------------------------------------------------------------
		class TCanMsg
		{
		public:
			static uint8_t  constexpr MAX_DATA_LENGTH = 8;

		public:
			TCanMsg(uint32_t const can_id, bool AIsRequest, uint8_t const can_data_len, uint8_t const * can_data_ptr) :
				id{ can_id },
				IsRequest{ AIsRequest },
				data_length{MitovMin(can_data_len, MAX_DATA_LENGTH)},
				data{0}
			{
				memcpy(data, can_data_ptr, data_length);
			}

			TCanMsg() : TCanMsg(0, false, 0, nullptr) { }

			TCanMsg(TCanMsg const & other)
			{
				this->id = other.id;
				this->IsRequest = other.IsRequest;
				this->data_length = other.data_length;
				memcpy(this->data, other.data, this->data_length);
			}

		public:
			TCanMsg & operator = (TCanMsg const & other)
			{
				if (this != &other)
				{
					this->id          = other.id;
					this->data_length = other.data_length;
					memcpy(this->data, other.data, this->data_length);
				}

				return (*this);
			}

			inline uint32_t getStandardId() const 
			{
				return (id & CAN_SFF_MASK);
			}
			inline uint32_t getExtendedId() const 
			{
				return (id & CAN_EFF_MASK);
			}
			inline bool isStandardId() const 
			{
				return ((id & CAN_EFF_FLAG) == 0);
			}

			inline bool isExtendedId() const 
			{
				return ((id & CAN_EFF_FLAG) == CAN_EFF_FLAG);
			}


			/*
			* CAN ID semantics (mirroring definition by linux/can.h):
			* |- Bit   31 : frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
			* |- Bit   30 : reserved (future remote transmission request flag)
			* |- Bit   29 : reserved (future error frame flag)
			* |- Bit 0-28 : CAN identifier (11/29 bit)
			*/
			uint32_t	id;
			uint8_t	data_length : 4;
			bool		IsRequest : 1;
			uint8_t	data[MAX_DATA_LENGTH];
		};
//---------------------------------------------------------------------------
		class TCanMsgRingbuffer
		{
		public:
			static size_t constexpr RING_BUFFER_SIZE = 32U;

			inline TCanMsgRingbuffer() : 
				_head{0},
				_tail{0},
				_num_elems{0}
 			{
			}

			inline bool isFull() const { return (_num_elems == RING_BUFFER_SIZE); }
			void enqueue(TCanMsg const & msg)
			{
				if (isFull())
					return;

				_buf[ _head ] = msg;
				_head = next(_head);
				_num_elems = _num_elems + 1;
			}

			inline bool isEmpty() const { return (_num_elems == 0); }
			TCanMsg dequeue()
			{
				if (isEmpty())
					return TCanMsg();

				TCanMsg const msg = _buf[ _tail ];
				_tail = next(_tail);
				_num_elems = _num_elems - 1;

				return msg;
			}

			inline size_t available() const { return _num_elems; }

		private:
			TCanMsg _buf[ RING_BUFFER_SIZE ];
			volatile size_t _head;
			volatile size_t _tail;
			volatile size_t _num_elems;

			inline size_t next(size_t const idx) const { return ((idx + 1) % RING_BUFFER_SIZE); }
		};
//---------------------------------------------------------------------------
		class TSyncCanMsgRingbuffer
		{
		private:
			TCanMsgRingbuffer _can_msg_buf;

		public:
			bool isFull() const { synchronized { _can_msg_buf.isFull(); } }
			void enqueue(TCanMsg const & msg) { synchronized { _can_msg_buf.enqueue(msg); } }

			bool isEmpty() const { synchronized { return _can_msg_buf.isEmpty(); } }
			TCanMsg dequeue() { synchronized { return _can_msg_buf.dequeue(); } }

			size_t available() const { synchronized { return _can_msg_buf.available(); } }

		public:
			TSyncCanMsgRingbuffer() : _can_msg_buf{} { }

		};
//---------------------------------------------------------------------------
        inline uint32_t CanStandardId(uint32_t const id) 
        {
			return (id & CAN_SFF_MASK);
        }

        inline uint32_t CanExtendedId(uint32_t const id) 
        {
			return ( CAN_EFF_FLAG | (id & CAN_EFF_MASK));
        }
//---------------------------------------------------------------------------
	}
//---------------------------------------------------------------------------
    namespace TArduino_UNO_R4_CanBus_TestMode
    {
        enum TArduino_UNO_R4_CanBus_TestMode
        {
			None,
			Listen,
			ExternalLoopback,
			InternalLoopback,
			InternalBus
        };
    }
//---------------------------------------------------------------------------
	template <
		typename T_FailedOutputPin,
		typename T_FSendTime,
		typename T_FSendingID,
		typename T_ID,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class TArduino_UNO_R4_CanBusSendingImplementation :
		public T_FailedOutputPin,
		public T_FSendTime,
		public T_FSendingID,
		public T_ID,
		public T_SendTimeout,
		public T_SentOutputPin
	{
	public:
		_V_PIN_( FailedOutputPin )
		_V_PIN_( SentOutputPin )

	public:
		_V_PROP_( SendTimeout )
		_V_PROP_( ID )

	protected:
		_V_PROP_( FSendTime )
		_V_PROP_( FSendingID )

	protected:
		volatile bool FSending = false; // Do not template to allow multithreading!
		volatile bool FSendFailed = false; // Do not template to allow multithreading!
		volatile bool FMessageSent = false; // Do not template to allow multithreading!

	protected:
		void ProcessStatus()
		{
			if( FMessageSent )
			{
//				Serial.println( "FSent" );
				T_SentOutputPin::ClockPin();
				FSending = false;
				FMessageSent = false;
			}

			if( FSendFailed )
			{
//				Serial.println( "FSendFailed" );
//				Serial.println( FSendFailed );
				T_FailedOutputPin::ClockPin();
				FSending = false;
				FSendFailed = false;
			}
		}

		inline void WaitForSending()
		{
			while( FSending )
			{
				delay( 1 );
				if( ! FMessageSent )
					if( millis() - FSendTime().GetValue() > SendTimeout().GetValue() )
					{
						T_FailedOutputPin::ClockPin();
						FSending = false;
						break;
					}

				ProcessStatus();
			}
		}

	public:
		inline void SystemLoopBegin()
		{
			if( ! FSending )
				return;

			ProcessStatus();
		}

	public:
		inline void MessageSendCompeled( uint32_t AID, bool ASuccess )
		{
			if( ! FSending )
				return;

			if( T_FSendingID::GetIsConstant() )
			{
				if( AID != FSendingID().GetValue() )
					return;
			}

			else
			{
				if( AID != ID().GetValue() )
					return;
			}

			if( ASuccess )
				FMessageSent = true;

			else
				FSendFailed = true;

		}

		inline void IsSending( uint32_t AID, bool &AResult )
		{
			WaitForSending();
			if( ! FSending )
				return;

			if( AResult )
				return;

			if( T_FSendingID::GetIsConstant() )
				AResult = ( AID == FSendingID().GetValue() );

			else
				AResult = ( AID == ID().GetValue() );

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_BusSpeed,
		typename T_Elements_IsSending,
		typename T_Elements_MessageSendCompeled,
		typename T_Elements_ProcessMessage,
		typename T_Enabled,
		typename T_ExtendedOutputPin,
//		typename T_FMessageSent,
//		typename T_FSendFailed,
		typename T_FSendTime,
		typename T_FSendingID,
//		typename T_FSending,
		typename T_IDOutputPin,
		typename T_MessageOutputPin,
		typename T_OutputPin,
		typename T_RequestOutputPin,
		typename T_Sending,
		typename T_TestMode
    > class TArduino_UNO_R4_CANBus_Module :
		public TArduino_UNO_R4_CanBusSendingImplementation <
				typename T_Sending::T_T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				Mitov::ConstantProperty<621, uint32_t, 0 >,
				typename T_Sending::T_T_SendTimeout,
				typename T_Sending::T_T_SentOutputPin
			>,
		public T_BusSpeed,
		public T_Enabled,
		public T_ExtendedOutputPin,
//		public T_FMessageSent,
//		public T_FSendFailed,
//		public T_FSendTime,
//		public T_FSending,
		public T_IDOutputPin,
		public T_MessageOutputPin,
		public T_OutputPin,
		public T_RequestOutputPin,
		public T_Sending,
		public T_TestMode
	{
		typedef TArduino_UNO_R4_CanBusSendingImplementation <
				typename T_Sending::T_T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				Mitov::ConstantProperty<621, uint32_t, 0 >,
				typename T_Sending::T_T_SendTimeout,
				typename T_Sending::T_T_SentOutputPin
			> inherited;
            
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( IDOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( BusSpeed )
		_V_PROP_( Enabled )
		_V_PROP_( Sending )
		_V_PROP_( TestMode )

	protected:
		static size_t constexpr CAN_MAX_NO_MAILBOXES = 32u;
		static uint32_t constexpr CAN_DEFAULT_MASK = 0x1FFFFFFFu;

	protected:
		can_instance_ctrl_t _can_ctrl;
		can_bit_timing_cfg_t _can_bit_timing_cfg;

		uint32_t _can_mailbox_mask[ CAN_MAX_NO_MAILBOXES / 4 ];
		can_mailbox_t _can_mailbox[ CAN_MAX_NO_MAILBOXES ];
		can_extended_cfg_t _can_extended_cfg;
		can_cfg_t _can_cfg;

		UNO_R4_CANBus::TSyncCanMsgRingbuffer _can_rx_buf;

	protected:
		void onCanCallback(can_callback_args_t * p_args)
		{
		  switch (p_args->event)
		  {
			case CAN_EVENT_TX_COMPLETE: 
			{
				inherited::MessageSendCompeled( p_args->frame.id, true );
				T_Elements_MessageSendCompeled::Call( p_args->frame.id, true );

//				Serial.println( "SEND:" );
//				Serial.println( p_args->frame.options );
				break;
			}

			case CAN_EVENT_RX_COMPLETE: // Currently driver don't support this. This is unreachable code for now.
			{
				// Extract the received CAN message.
				UNO_R4_CANBus::TCanMsg const msg
				(
					(p_args->frame.id_mode == CAN_ID_MODE_STANDARD) ? UNO_R4_CANBus::CanStandardId(p_args->frame.id) : UNO_R4_CANBus::CanExtendedId(p_args->frame.id),
					p_args->frame.type == CAN_FRAME_TYPE_REMOTE,
					p_args->frame.data_length_code,
					p_args->frame.data
				);

				// Store the received CAN message in the receive buffer.
				_can_rx_buf.enqueue( msg );
				break;
			}

			case CAN_EVENT_TX_ABORTED:           // Transmit abort event.
			{
//				Serial.println( "ABBORT!" );
				inherited::MessageSendCompeled( p_args->frame.id, false );
				T_Elements_MessageSendCompeled::Call( p_args->frame.id, false );
				break;
			}

			case CAN_EVENT_ERR_WARNING:          // error warning event
			case CAN_EVENT_ERR_PASSIVE:          // error passive event
			case CAN_EVENT_ERR_BUS_OFF:          // error bus off event
			case CAN_EVENT_BUS_RECOVERY:         // Bus recovery error event
			case CAN_EVENT_MAILBOX_MESSAGE_LOST: // overwrite/overrun error event
			case CAN_EVENT_ERR_BUS_LOCK:         // Bus lock detected (32 consecutive dominant bits).
			case CAN_EVENT_ERR_CHANNEL:          // Channel error has occurred.

			case CAN_EVENT_ERR_GLOBAL:           // Global error has occurred.
			case CAN_EVENT_TX_FIFO_EMPTY:        // Transmit FIFO is empty.
			{
//			  _is_error = true;
//			  _err_code = p_args->event;
			  break;
			}
		  }
		}

		static void can_callback( can_callback_args_t * p_args )
		{
			((TArduino_UNO_R4_CANBus_Module *)(p_args->p_context))->onCanCallback(p_args);
		}

		bool cfg_pins(int const max_index, int const can_tx_pin, int const can_rx_pin)
		{
			/* Verify if indices are good. */
			if (can_tx_pin < 0 || can_rx_pin < 0 || can_tx_pin >= max_index || can_rx_pin >= max_index)
				return false;

			/* Getting configuration from table. */
			const uint16_t * cfg = nullptr;
			auto cfg_can_tx = getPinCfgs(can_tx_pin, PIN_CFG_REQ_CAN_TX);
			auto cfg_can_rx = getPinCfgs(can_rx_pin, PIN_CFG_REQ_CAN_RX);

			/* Verify if configurations are good. */
			if (cfg_can_tx[0] == 0 || cfg_can_rx[0] == 0)
				return false;

			/* Verify if channel is the same for all pins. */
			uint32_t const ch_can_tx = GET_CHANNEL(cfg_can_tx[0]);
			uint32_t const ch_can_rx = GET_CHANNEL(cfg_can_rx[0]);
			if (ch_can_tx != ch_can_rx)
				return false;

			/* Actually configure pin functions. */
			R_IOPORT_PinCfg( &g_ioport_ctrl, g_pin_cfg[can_tx_pin].pin, uint32_t(IOPORT_CFG_PERIPHERAL_PIN) | uint32_t(IOPORT_PERIPHERAL_CAN) );
			R_IOPORT_PinCfg( &g_ioport_ctrl, g_pin_cfg[can_rx_pin].pin, uint32_t(IOPORT_CFG_PERIPHERAL_PIN) | uint32_t(IOPORT_PERIPHERAL_CAN) );

			return true;
		}

		bool begin(CanBitRate const can_bitrate)
		{
			bool init_ok = true;

			/* Configure the pins for CAN.
			*/
			int const max_index = PINS_COUNT;
			init_ok &= cfg_pins(max_index, PIN_CAN0_TX, PIN_CAN0_RX );

			/* Configure the interrupts.
			*/
			CanIrqReq_t irq_req
			{
				.ctrl = &_can_ctrl,
				.cfg = &_can_cfg,
			};

			init_ok &= IRQManager::getInstance().addPeripheral(IRQ_CAN, &irq_req);

			/* Calculate the CAN bitrate based on the value of this functions parameter.
			*/
			uint32_t constexpr F_CAN_CLK_Hz = 24*1000*1000UL;
			uint32_t constexpr TQ_MIN     = 8;
			uint32_t constexpr TQ_MAX     = 25;
			uint32_t constexpr TSEG_1_MIN = 4;
			uint32_t constexpr TSEG_1_MAX = 16;
			uint32_t constexpr TSEG_2_MIN = 2;
			uint32_t constexpr TSEG_2_MAX = 8;

			auto [is_valid_baudrate, baud_rate_prescaler, time_segment_1, time_segment_2] =
				util::calc_can_bit_timing(can_bitrate, F_CAN_CLK_Hz, TQ_MIN, TQ_MAX, TSEG_1_MIN, TSEG_1_MAX, TSEG_2_MIN, TSEG_2_MAX);

			init_ok &= is_valid_baudrate;

			if (is_valid_baudrate) 
			{
				_can_bit_timing_cfg.baud_rate_prescaler = baud_rate_prescaler;
				_can_bit_timing_cfg.time_segment_1 = time_segment_1;
				_can_bit_timing_cfg.time_segment_2 = time_segment_2;
				_can_bit_timing_cfg.synchronization_jump_width = 1;
			}

			/* Initialize the peripheral's FSP driver. */
			if (R_CAN_Open(&_can_ctrl, &_can_cfg) != FSP_SUCCESS)
				init_ok = false;

			return init_ok;
		}

	public:
		inline void UpdateTestMode()
		{
			if( ! Enabled().GetValue() )
				return;

			can_test_mode_t ATestMode;

			switch( TestMode() )
			{
				case TArduino_UNO_R4_CanBus_TestMode::Listen : ATestMode = CAN_TEST_MODE_LISTEN; break;
				case TArduino_UNO_R4_CanBus_TestMode::ExternalLoopback : ATestMode = CAN_TEST_MODE_LOOPBACK_EXTERNAL; break;
				case TArduino_UNO_R4_CanBus_TestMode::InternalLoopback : ATestMode = CAN_TEST_MODE_LOOPBACK_INTERNAL; break;
				case TArduino_UNO_R4_CanBus_TestMode::InternalBus : ATestMode = CAN_TEST_MODE_INTERNAL_BUS; break;

				default: ATestMode = CAN_TEST_MODE_DISABLED; break;
			}	

			R_CAN_ModeTransition( &_can_ctrl, CAN_OPERATION_MODE_NORMAL, ATestMode );
		}

		void UpdateEnabled()
		{
			if( Enabled() )
			{
				CanBitRate ARate;
				if( BusSpeed() < 250 )
					ARate = CanBitRate::BR_125k;

				else if( BusSpeed() < 500 )
					ARate = CanBitRate::BR_250k;

				else if( BusSpeed() < 1000 )
					ARate = CanBitRate::BR_500k;

				else
					ARate = CanBitRate::BR_1000k;

				begin( ARate );
				UpdateTestMode();
			}

			else
			  R_CAN_Close( &_can_ctrl );

		}

	public:
		bool SendMessage( uint32_t id, bool AExtended, bool ARequest, uint8_t len, const uint8_t *buf ) //, bool wait_sent )
		{
			if( ! Enabled().GetValue() )
				return false;

			for(;;)
			{
				bool AIsSendingID = false;
				inherited::IsSending( id, AIsSendingID );
				if( ! AIsSendingID )
					T_Elements_IsSending::Call( id, AIsSendingID );

				if( ! AIsSendingID )
					break;

				delay( 1 );
			}

//			UNO_R4_CANBus::TCanMsg AMsg( id, ARequest, len, buf );
//			CAN.write( AMsg );

//			bool const is_standard_id = AMsg.isStandardId();

			can_frame_t can_msg = {
				AExtended ? (id & UNO_R4_CANBus::CAN_EFF_MASK) : ( id & UNO_R4_CANBus::CAN_SFF_MASK ), // id
//				/* id               = */ is_standard_id ? AMsg.getStandardId()  : AMsg.getExtendedId(),
				AExtended ? CAN_ID_MODE_EXTENDED : CAN_ID_MODE_STANDARD,
//				/* id_mode          = */ is_standard_id ? CAN_ID_MODE_STANDARD : CAN_ID_MODE_EXTENDED,
				ARequest ? CAN_FRAME_TYPE_REMOTE : CAN_FRAME_TYPE_DATA, // type
				min( len, CAN_DATA_BUFFER_LENGTH), // data_length_code
				0 // options
			};

			memcpy( can_msg.data, buf, can_msg.data_length_code );

			R_CAN_Write( &_can_ctrl, AExtended ? CAN_MAILBOX_ID_16 : CAN_MAILBOX_ID_0, &can_msg );

			return true;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            inherited::WaitForSending();

			Mitov::TCanBusPacket &ADataBlock = *(Mitov::TCanBusPacket*)_Data;
			if( SendMessage( ADataBlock.GetID(), ADataBlock.IsExtended(), ADataBlock.IsRequest(), ADataBlock.GetSize(), ADataBlock.Read() ))
			{
				inherited::FSending = true;
				inherited::FSendingID() = ADataBlock.GetID();
				inherited::FSendTime() = millis();
			}
		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				UpdateEnabled();

		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled().GetValue() )
				return;

			if( ! _can_rx_buf.available() )
				return;

//			Serial.println( "TEST1" );

			UNO_R4_CANBus::TCanMsg const AMsg = _can_rx_buf.dequeue();

			T_Elements_ProcessMessage::Call( AMsg.id, AMsg.getExtendedId(), AMsg.IsRequest, AMsg.data_length, AMsg.data );
			T_IDOutputPin::SetPinValue( AMsg.id );
			T_ExtendedOutputPin::SetPinValue( AMsg.getExtendedId() );

			T_RequestOutputPin::SetPinValue( AMsg.IsRequest );

			if( T_MessageOutputPin::GetPinIsConnected() )
			{
				TDataBlock AData( AMsg.data_length, AMsg.data );
				T_MessageOutputPin::SetPinValue( AData );
			}

			if( T_OutputPin::GetPinIsConnected() )
			{
                Mitov::TCanBusPacket AData( AMsg.id, AMsg.getExtendedId(), AMsg.IsRequest, AMsg.data_length, AMsg.data );
				T_OutputPin::SetPinValue( AData );
			}
		}

	public:
		inline TArduino_UNO_R4_CANBus_Module() :
			_can_bit_timing_cfg{} ,
			_can_mailbox_mask
				{
				CAN_DEFAULT_MASK,
				CAN_DEFAULT_MASK,
				CAN_DEFAULT_MASK,
				CAN_DEFAULT_MASK,
				0,                /* Use no id filtering -> a CAN frame with any ID will be stored in receive mailbox group #0. */
				CAN_DEFAULT_MASK,
				0,                /* Use no id filtering -> a CAN frame with any ID will be stored in receive mailbox group #2. */
				CAN_DEFAULT_MASK
				},
			_can_mailbox
				{
				  /* Mailbox Group #0 */
				  { .mailbox_id =  0, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id =  1, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id =  2, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id =  3, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  /* Mailbox Group #1 */
				  { .mailbox_id =  4, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id =  5, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id =  6, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id =  7, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  /* Mailbox Group #2 */
				  { .mailbox_id =  8, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id =  9, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id = 10, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id = 11, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  /* Mailbox Group #3 */
				  { .mailbox_id = 12, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id = 13, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id = 14, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  { .mailbox_id = 15, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_TRANSMIT },
				  /* We only use the very first receive mailbox for receiving. */
				  /* Mailbox Group #4 */
				  { .mailbox_id =  0, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id =  1, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id =  2, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id =  3, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  /* Mailbox Group #5 */
				  { .mailbox_id =  4, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id =  5, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id =  6, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id =  7, .id_mode = CAN_ID_MODE_EXTENDED, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  /* Mailbox Group #6 */
				  { .mailbox_id =  8, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id =  9, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id = 10, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id = 11, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  /* Mailbox Group #7 */
				  { .mailbox_id = 12, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id = 13, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id = 14, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE },
				  { .mailbox_id = 15, .id_mode = CAN_ID_MODE_STANDARD, .frame_type = CAN_FRAME_TYPE_DATA, .mailbox_type = CAN_MAILBOX_RECEIVE }
				},
			_can_extended_cfg
				{
				  .clock_source   = CAN_CLOCK_SOURCE_PCLKB,
				  .p_mailbox_mask = _can_mailbox_mask,
				  .p_mailbox      = _can_mailbox,
				  .global_id_mode = CAN_GLOBAL_ID_MODE_MIXED,
				  .mailbox_count  = CAN_MAX_NO_MAILBOXES,
				  .message_mode   = CAN_MESSAGE_MODE_OVERWRITE,
				  .p_fifo_int_cfg = nullptr,
				  .p_rx_fifo_cfg  = nullptr,
				},
			_can_cfg
				{
				  .channel        = 0,
				  .p_bit_timing   = &_can_bit_timing_cfg,
				  .p_callback     = can_callback,
				  .p_context      = this,
				  .p_extend       = &_can_extended_cfg,
				  .ipl            = (12),
				  .error_irq      = FSP_INVALID_VECTOR,
				  .rx_irq         = FSP_INVALID_VECTOR,
				  .tx_irq         = FSP_INVALID_VECTOR,
				}
		{
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Extended,
        typename T_FSendTime,
		typename T_FSendingID,
//		typename T_FMessageSent,
//		typename T_FSendFailed,
//		typename T_FSending,
		typename T_FailedOutputPin,
		typename T_ID,
//		uint8_t C_INDEX,
        typename T_InitialValue,
		typename T_Request,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class TArduinoCanBusSendMessage_UNO_R4_Element :
		public TArduino_UNO_R4_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			>,
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
//		public T_FMessageSent,
//		public T_FSendFailed,
//		public T_FSending,
//		public T_ID,
        public T_InitialValue,
		public T_Request
	{
        typedef TArduino_UNO_R4_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			> inherited;
            
	public:
		_V_PROP_( Extended )
//		_V_PROP_( ID )
		_V_PROP_( InitialValue )
		_V_PROP_( Request )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
            inherited::WaitForSending();

            const uint8_t *ABuffer = T_InitialValue::Allocate();
   			if( C_OWNER.SendMessage( inherited::ID().GetValue(), Extended().GetValue(), Request().GetValue(), T_InitialValue::GetCount(), ABuffer ))
			{
				inherited::FSending = true;
				inherited::FSendingID() = inherited::ID().GetValue();
				inherited::FSendTime() = millis();
			}

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
        typename T_FSendTime,
		typename T_FSendingID,
		typename T_FailedOutputPin,
		typename T_ID,
        typename T_InitialValue,
		typename T_MessageOutputPin,
		typename T_RequestOutputPin,
		typename T_SendTimeout,
		typename T_SentOutputPin
	> class Microchip_UNO_R4_CanBus_RequestMessage : 
		public TArduino_UNO_R4_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			>,
		public T_ClockInputPin_o_IsConnected,
		public T_Extended,
		public T_ExtendedOutputPin,
        public T_InitialValue,
		public T_MessageOutputPin,
		public T_RequestOutputPin
	{
        typedef TArduino_UNO_R4_CanBusSendingImplementation <
				T_FailedOutputPin,
                T_FSendTime,
				T_FSendingID,
				T_ID,
				T_SendTimeout,
				T_SentOutputPin
			> inherited;
            
	public:
		_V_PIN_( ExtendedOutputPin )
		_V_PIN_( MessageOutputPin )
		_V_PIN_( RequestOutputPin )

	public:
		_V_PROP_( Extended )
		_V_PROP_( InitialValue )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
            inherited::WaitForSending();

            const uint8_t *ABuffer = T_InitialValue::Allocate();
   			if( C_OWNER.SendMessage( inherited::ID().GetValue(), Extended().GetValue(), true, T_InitialValue::GetCount(), ABuffer ))
			{
				inherited::FSending = true;
				inherited::FSendingID() = inherited::ID().GetValue();
				inherited::FSendTime() = millis();
			}

            T_InitialValue::Release( ABuffer );
		}

		inline void MessageInputPin_o_Receive( void *_Data )
		{
			InitialValue = *(Mitov::TDataBlock*)_Data;
			if( ! ClockInputPin_o_IsConnected().GetValue() )
                ClockInputPin_o_Receive( nullptr );

		}

	public:
		inline void ProcessMessage( uint32_t AID, bool AExtended, bool ARequest, uint8_t  ALength, const uint8_t * ABuffer )
		{
			if( AID != inherited::ID().GetValue() )
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
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif