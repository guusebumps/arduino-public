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
#include "freertos/ringbuf.h"
#include <Mitov_FreeRTOS.h>

//namespace esp_i2s
//{
 #include <driver/i2s.h> // ESP specific i2s driver
//}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#define USBSerial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
	    typename T_Count,
	    typename T_Size
    > class TArduinoBasicI2S_Buffers :
	    public T_Count,
	    public T_Size
    {
    public:
		_V_PROP_( Count )
		_V_PROP_( Size )

    };
//---------------------------------------------------------------------------
	template <
        typename T_AutoConfig,
        typename T_Rate
    > class TArduinoBasicI2S_SampleRate :
        public T_AutoConfig,
        public T_Rate
    {
    public:
		_V_PROP_( AutoConfig )
		_V_PROP_( Rate )

    };
//---------------------------------------------------------------------------
	template <
        typename T_AutoConfig,
        typename T_Count
    > class TArduinoBasicI2S_Bits :
        public T_AutoConfig,
        public T_Count
    {
    public:
		_V_PROP_( AutoConfig )
		_V_PROP_( Count )

    };
//---------------------------------------------------------------------------
	template <
        typename T_AutoConfig,
        typename T_Mode
    > class TArduinoBasicI2S_Mode :
        public T_AutoConfig,
        public T_Mode
    {
    public:
		_V_PROP_( AutoConfig )
		_V_PROP_( Mode )

    };
//---------------------------------------------------------------------------
	template <
        typename T_Enabled,
        typename T_PinNumber
    > class TArduino_ESP32_I2S_ExternalClocking :
        public T_Enabled,
        public T_PinNumber
    {
    public:
		_V_PROP_( Enabled )
		_V_PROP_( PinNumber )

    };
//---------------------------------------------------------------------------
    namespace TArduinoBasicI2S_ModeEnum
    {
        enum TArduinoBasicI2S_ModeEnum
        {
            Standard = TI2SMode::Standard, // Phillips
            MSB = TI2SMode::MSB,
            ShortPCM = TI2SMode::ShortPCM,
            LongPCM = TI2SMode::LongPCM,
            PDM = TI2SMode::PDM            // Pulse Density Modulation
        };
    }
//---------------------------------------------------------------------------
	template <
	    typename T_Buffers,
	    typename T_DataInputPin_o_IsConnected,
	    typename T_InterruptLevel,
	    typename T_OutputPin
    > class TArduinoESP32_I2S_InputChannel :
	    public T_Buffers,
	    public T_DataInputPin_o_IsConnected,
	    public T_InterruptLevel,
	    public T_OutputPin
    {
    public:
		_V_PROP_( OutputPin )

    public:
		_V_PROP_( Buffers )
		_V_PROP_( InterruptLevel )

    public:
		_V_PROP_( DataInputPin_o_IsConnected )

    };
//---------------------------------------------------------------------------
	template <
	    typename T_Buffers,
	    typename T_InterruptLevel
    > class TArduinoESP32_I2S_OutputChannel :
	    public T_Buffers,
	    public T_InterruptLevel
    {
    public:
		_V_PROP_( Buffers )
		_V_PROP_( InterruptLevel )

	};
//---------------------------------------------------------------------------
    namespace ESP32_I2S_Const
    {
        const uint8_t C_I2S_EVENT_QUEUE_LENGTH = 16;
    }
//---------------------------------------------------------------------------
	template <
		typename T_Bits,
		typename T_ClockPin,
		typename T_ControlOutputPin,
		typename T_DataIn,
//		typename T_DataInputPin_o_IsConnected,
		typename T_DataOut,
		typename T_ExternalClocking,
		typename T_FBytesPerSample,
		typename T_FCountChannels,
		typename T_FInputBuffersCount,
		typename T_FIsSecondChannel,
		typename T_FOutputBuffersCount,
		uint8_t C_INDEX,
		typename T_INSTANCE_DmaTransferComplete,
//		typename T_INSTANCE_DmaInputTransferComplete,
//		typename T_INSTANCE_DmaOutputTransferComplete,
		typename T_Input,
		typename T_IsSlave,
		typename T_Mode,
		typename T_Output,
		typename T_SampleRate,
		typename T_WordSelectPin
    > class TArduino_ESP32_I2S :
		public T_Bits,
		public T_ClockPin,
		public T_ControlOutputPin,
		public T_DataIn,
//		public T_DataInputPin_o_IsConnected,
		public T_DataOut,
		public T_ExternalClocking,
		public T_FBytesPerSample,
		public T_FCountChannels,
		public T_FInputBuffersCount,
		public T_FIsSecondChannel,
		public T_FOutputBuffersCount,
		public T_Input,
		public T_IsSlave,
		public T_Mode,
		public T_Output,
		public T_SampleRate,
		public T_WordSelectPin
    {
    public:
		_V_PROP_( ControlOutputPin )

    public:
		_V_PROP_( Bits )
		_V_PROP_( ClockPin )
		_V_PROP_( DataIn )
		_V_PROP_( DataOut )
		_V_PROP_( ExternalClocking )
		_V_PROP_( Input )
		_V_PROP_( IsSlave )
		_V_PROP_( Mode )
		_V_PROP_( Output )
		_V_PROP_( SampleRate )
		_V_PROP_( WordSelectPin )

	protected:
	    _V_PROP_( FBytesPerSample )
	    _V_PROP_( FCountChannels )
	    _V_PROP_( FInputBuffersCount )
		_V_PROP_( FIsSecondChannel )
	    _V_PROP_( FOutputBuffersCount )

        uint32_t FInputSampleRate;
        uint32_t FOutputSampleRate;

	protected:
        RingbufHandle_t FInputRingBuffer = nullptr;
        RingbufHandle_t FOutputRingBuffer = nullptr;

        TaskHandle_t    FCallbackTaskHandle = nullptr;
//        TaskHandle_t    FCallbackTaskInputHandle = nullptr;
//        TaskHandle_t    FCallbackTaskOutputHandle = nullptr;
        QueueHandle_t   FEventQueue = nullptr;
//        QueueHandle_t   FEventInputQueue = nullptr;
//        QueueHandle_t   FEventOutputQueue = nullptr;

        uint32_t        FInputBufferSize;
        uint32_t        FOutputBufferSize;

        uint32_t        FChannelMap;

        TMutex FMutex; // = nullptr;

        typedef void (* TCallback)( void *AInstance, void *_Data );
        struct TCallbackEntry
        {
            void * Instance;
            TCallback Callback;
        };

        ::Mitov::SimpleList<TCallbackEntry> FControlCallbacks;
        ::Mitov::SimpleList<TCallbackEntry> FOutputCallbacks;

    public:
        inline void AddControlCallback( void *AInstance, TCallback AFunc )
        {
            FControlCallbacks.push_back( { AInstance, AFunc } );
        }

        inline void AddOutputCallback( void *AInstance, TCallback AFunc )
        {
            FOutputCallbacks.push_back( { AInstance, AFunc } );
        }

	protected:
	    void ConfigDirection( TI2SDirectionInfo &ADirection, i2s_config_t &AConfig, TI2SMode::TI2SMode *AFormats, int AFormatsCount, bool ARatePopulated, bool ABitsPopulated, bool AModePopulated, uint32_t &AChannelMap )
	    {
            if( ADirection.Used )
            {
                for( int i = 0; i < AFormatsCount; ++ i )
                {
                    if( ADirection.Modes[ i ].Enabled )
                    {
//                        Serial.print( "Modes: " );
//                        Serial.println( i );
//                        Serial.println( ADirection.Modes[ i ].MaxSampleRate );

                        if( ! ARatePopulated )
                            AConfig.sample_rate = ADirection.Modes[ i ].MaxSampleRate;

                        if( ! ABitsPopulated )
                        {
                            if( ADirection.Modes[ i ].BitsPerSample > 24 )
                                AConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;

                            else if( ADirection.Modes[ i ].BitsPerSample > 16 )
                                AConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_24BIT;

                            else if( ADirection.Modes[ i ].BitsPerSample > 8 )
                                AConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;

                            else
                                AConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT;

                        }

                        if( AFormats[ i ] == TI2SMode::PDM )
                            AConfig.mode = i2s_mode_t( AConfig.mode | I2S_MODE_PDM );

                        AChannelMap = ADirection.Modes[ i ].ChannelsMap;
//                        USBSerial.print( "AChannelMap: " );
//                        USBSerial.print( i );
//                        USBSerial.print( " : " );
//                        USBSerial.println( AChannelMap, BIN );
                        switch ( AChannelMap & 0b11 )
                        {
                            case 2 : AConfig.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT; break;
                            case 3 : AConfig.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT; break;
                            default : AConfig.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT; break;
                        }

                        switch( ADirection.Modes[ i ].Mode )
                        {
                            default:
                            case TI2SMode::Standard :
                            case TI2SMode::PDM :
                            case TI2SMode::TDM :
                                AConfig.communication_format = I2S_COMM_FORMAT_STAND_I2S;
                                break;

                            case TI2SMode::MSB :
                                AConfig.communication_format = I2S_COMM_FORMAT_STAND_MSB;
                                break;

                            case TI2SMode::ShortPCM :
                                AConfig.communication_format = I2S_COMM_FORMAT_STAND_PCM_SHORT;
                                break;

                            case TI2SMode::LongPCM :
                                AConfig.communication_format = I2S_COMM_FORMAT_STAND_PCM_LONG;
                                break;

                        }

                        break;
                    }
                }

//                 =
//                {
//                    .mode = (i2s_mode_t)(I2S_MODE_MASTER),  // Set the I2S operating mode.
//                    .sample_rate =

//                };
            }
	    }

        static void onDmaComplete(void *)
        {
            T_INSTANCE_DmaTransferComplete::Call();
        }
/*
        static void onDmaInputComplete(void *)
        {
            T_INSTANCE_DmaInputTransferComplete::Call();
        }

        static void onDmaOutputComplete(void *)
        {
            T_INSTANCE_DmaOutputTransferComplete::Call();
        }
*/
        inline i2s_port_t GetPortNumber()
        {
#if( SOC_I2S_NUM > 1 )
            return ( FIsSecondChannel() ) ? I2S_NUM_1 : I2S_NUM_0;
#else
            return I2S_NUM_0;
#endif
        }
    
        
        void rx_done_routine()
        {
//            Serial.println( "@" );
//            Serial.println( "rx_done_routine" );
            auto ALock = FMutex.GetLock();

            size_t bytes_read = 0;
//            const size_t single_dma_buf = _i2s_dma_buffer_size*(_bitsPerSample/8);

            if( FInputRingBuffer )
            {
//                Serial.println( FInputBufferSize * 4 );
                uint8_t *AInputBuffer = (uint8_t*)malloc( FInputBufferSize * 4 );
                size_t avail = xRingbufferGetCurFreeSize( FInputRingBuffer );
//                Serial.println( avail );
                if(avail > 0)
                {
                    esp_err_t ret = i2s_read( GetPortNumber(), AInputBuffer, avail <= FInputBufferSize ? avail : FInputBufferSize, (size_t*) &bytes_read, 0 );
//                    if(ret != ESP_OK)
//                        log_w("i2s_read returned with error %d", ret);

                    _post_read_data_fix( AInputBuffer, &bytes_read );
//                    Serial.println( "T1" );
                }

//                Serial.println( bytes_read );

                if( bytes_read > 0 )
                { // when read more than 0, then send to ring buffer
                    if( pdTRUE != xRingbufferSend( FInputRingBuffer, AInputBuffer, bytes_read, 0 ))
                        ;
//                        log_w("I2S failed to send item from DMA to internal buffer\n");

                }

                free( AInputBuffer );
//                if (_onReceive && avail < _buffer_byte_size) // when user callback is registered && and there is some data in ring buffer to read
//                  _onReceive();

            }
        }

        void _post_read_data_fix(void *input, size_t *size)
        {
            uint32_t dst_ptr = 0;

//            USBSerial.println( "*" );

//            uint32_t ATest = 0;
            switch( GetBytesPerSample() )
            {
                case 1: // 8 bit
                    for( int i = 0; i < *size; i += 4 )
                    {
                        ((uint8_t*)input)[ dst_ptr ++ ] = ((uint8_t*)input)[ i + 3 ];
                        ((uint8_t*)input)[ dst_ptr ++ ] = ((uint8_t*)input)[ i + 1 ];
                    }

                    *size /= 2;
                    break;

                case 2: // 16 bit
                    uint16_t tmp;
                    for( int i = 0; i < *size / 2; i += 2 )
                    {
                        tmp = ((uint16_t*)input)[ i ];
                        ((uint16_t*)input)[ dst_ptr ++ ] = ((uint16_t*)input)[ i + 1 ];
                        ((uint16_t*)input)[ dst_ptr ++ ] = tmp;

//                        USBSerial.println( ((int16_t*)input)[ ATest ++ ] );
                    }

                    break;

/*
                case 3: // 24 bit
                    USBSerial.println( "TEST1" );
                    break;

                case 4: // 32 bit
                    USBSerial.println( "TEST2" );
                    break;
*/
                default: ; // Do nothing
            } // switch
        }

        // Prepares data and writes them to IDF i2s driver.
        // This counters possible bug in ESP IDF I2S driver
        // output - bytes to be sent
        // size - number of bytes in original buffer
        // bytes_written - number of bytes used from original buffer
        // actual_bytes_written - number of bytes written by i2s_write after fix
//        void _fix_and_write(void *output, size_t size, size_t *bytes_written = NULL, size_t *actual_bytes_written = NULL )
        void _fix_and_write(void *output, size_t size )
        {
            uint8_t *APtr = (uint8_t *)output;
            if( size )
                for(;;)
                {
                    size_t _bytes_written;
                    esp_err_t ret = i2s_write( GetPortNumber(), APtr, size, &_bytes_written, 0 );
/*
                    if( _bytes_written )
                    {
                        USBSerial.print( "WRITE: " );
                        USBSerial.println( _bytes_written );
                    }
*/
//                    esp_err_t ret = i2s_write( ( FIsSecondChannel() || ( FInputBuffersCount() && FOutputBuffersCount() ) ) ? I2S_NUM_1 : I2S_NUM_0, APtr, size, &_bytes_written, 0 );

                    if( size == _bytes_written )
                        break;

                    APtr += _bytes_written;
                    size -= _bytes_written;                    
                }

        }

    public:
        inline void DmaTransferComplete()
        {
//            Serial.println( "DmaInputTransferComplete" );
//            DmaTransferComplete( FEventInputQueue );
//Serial.println( "TEST_2" );
//return;
//            for(;;)
//                delay( 1000 );

            i2s_event_t AEvent;

            const size_t single_dma_buf = FOutputBufferSize * FCountChannels().GetValue(); // *2 for stereo - it has double number of samples for 2 channels
            const int AOutputBufferByteSize = FOutputBufferSize * FOutputBuffersCount() * FCountChannels().GetValue();

            while( true )
            {
//                Serial.println( "TEST_1" );
//delay( 1000 );
//return;
                xQueueReceive( FEventQueue, &AEvent, portMAX_DELAY );
//                Serial.println( "TEST_5" );

//                    Serial.println( uint64_t( FOutputRingBuffer ));

//                Serial.println( AEvent.type );
//                delay( 100 );
//delay( 1000 );
//continue;
                if( AEvent.type == I2S_EVENT_TX_DONE )
                {
//                    auto ALock = FMutex.GetLock();

//                    Serial.println( "TEST_7" );
//                    Serial.println( uint64_t( FOutputRingBuffer ));

//                    delay( 300 );

//                auto ATest1 = xRingbufferGetCurFreeSize( FOutputRingBuffer );
//                Serial.println( ATest1 );

                    if( FOutputRingBuffer )
                    {
                        size_t AFreeSize = xRingbufferGetCurFreeSize( FOutputRingBuffer );
//                        USBSerial.println( AFreeSize );
                        if( AOutputBufferByteSize - AFreeSize >= single_dma_buf )
                        { // fill up the I2S DMA buffer
    //                        Serial.println( "TEST_8" );
                            if( AOutputBufferByteSize - AFreeSize >= FOutputBufferSize )
                            { // don't read from almost empty buffer
                                size_t item_size = 0;
                                void *item = xRingbufferReceiveUpTo( FOutputRingBuffer, &item_size, pdMS_TO_TICKS( 0 ), single_dma_buf );
                                if( item != NULL )
                                {
                                    _fix_and_write( item, item_size );
                                    vRingbufferReturnItem( FOutputRingBuffer, item );
                                } // Check received item
                            } // don't read from almost empty buffer
                        } // fill up the I2S DMA buffer
                    }
                }

                else if( AEvent.type == I2S_EVENT_RX_DONE )
                    rx_done_routine();

            } // infinite loop
        }
        
/*
        inline void DmaInputTransferComplete()
        {
//            Serial.println( "DmaInputTransferComplete" );
            DmaTransferComplete( FEventInputQueue );
        }

        inline void DmaOutputTransferComplete()
        {
//            Serial.println( "DmaOutputTransferComplete" );
            DmaTransferComplete( FEventInputQueue );
//            DmaTransferComplete( FEventOutputQueue );
        }
*/
    protected:
        void SendToBufffer( RingbufHandle_t ABuffer, uint8_t *ADataPtr, uint8_t ABytesPerSample, uint32_t ALength )
        {
            if( ALength )
                for(;;)
                {
                    size_t AAvailable = xRingbufferGetCurFreeSize( FOutputRingBuffer );
                    AAvailable = MitovMin<size_t>( ALength, AAvailable );

                    AAvailable /= ABytesPerSample;
                    AAvailable *= ABytesPerSample;

//                    xRingbufferSend( FOutputRingBuffer, ADataPtr, AAvailable, portMAX_DELAY );
                    xRingbufferSend( FOutputRingBuffer, ADataPtr, AAvailable, 0 );
                    if( ALength == AAvailable )
                        break;

                    ADataPtr += AAvailable; 
                    ALength -= AAvailable;
                }

        }

        inline uint8_t GetBytesPerSample()
        {
            return FBytesPerSample().GetValue() + 1;
        }

	public:
		inline void AudioOutputChannel_InputPin_o_Receive( void *_Data )
		{
//            auto ALock = FMutex.GetLock();

            if( ! FOutputRingBuffer )
                return;

    		TAudioBuffer &ABuffer = *(TAudioBuffer *)_Data;

/*
            USBSerial.println( "W" );
            USBSerial.print( "BytesPerSample: " );
            USBSerial.print( GetBytesPerSample() );
            USBSerial.print( " - " );
            USBSerial.println( ( ABuffer.GetBitsPerSample() + 7 ) / 8 );

            USBSerial.print( "CountChannels: " );
            USBSerial.print( FCountChannels() );
            USBSerial.print( " - " );
            USBSerial.println( ABuffer.GetChannelsCount() );

            USBSerial.print( "OutputSampleRate: " );
            USBSerial.print( FOutputSampleRate );
            USBSerial.print( " - " );
            USBSerial.println( ABuffer.SampleRate );
*/
//            while( true )
//                delay( 1000 );

            if( ( GetBytesPerSample() == ( ABuffer.GetBitsPerSample() + 7 ) / 8 ) && ( FCountChannels() == ABuffer.GetChannelsCount() ) && ( FOutputSampleRate == ABuffer.GetSampleRate() ) )
                SendToBufffer( FOutputRingBuffer, (uint8_t*)ABuffer.Read(), ABuffer.GetBytesPerSample(), ABuffer.GetSize() );

            else
            {
				const uint8_t *ABaseInPtr = ABuffer.Read();
                auto ABytesPerSample = ABuffer.GetBytesPerSample();
				uint32_t AInStep = ABuffer.GetSampleStep();
				uint32_t AOutStep = TAudioBuffer::GetSampleStep( GetBytesPerSample(), FCountChannels() );
                auto AInChannelsCount = ABuffer.GetChannelsCount();
                uint8_t AZeroSize = ( FCountChannels().GetValue() - AInChannelsCount ) * GetBytesPerSample();

                if( FOutputSampleRate == ABuffer.GetSampleRate() )
                {
//                    Serial.print( "*" );
                    for( int i = 0; i < ABuffer.GetSize(); ++ i )
                    {
                        uint8_t AData[ 4 ];
//                        USBSerial.println( *(int16_t *)ABaseInPtr );
					    TAudioBuffer::CopySample( ABaseInPtr, AData, AInStep, AOutStep, AInChannelsCount, FCountChannels(), ABytesPerSample, GetBytesPerSample(), ABuffer.GetBitsPerSample(), GetBytesPerSample() * 8, AZeroSize );
//                        USBSerial.println( *(int16_t *)AData );
//                        USBSerial.println( GetBytesPerSample() );
                        SendToBufffer( FOutputRingBuffer, AData, GetBytesPerSample(), GetBytesPerSample() );
					    ABaseInPtr += AInStep;
                    }
                }
                else
                {
				    float ARatio = float( FOutputSampleRate ) / ABuffer.GetSampleRate();
				    float AReverseRatio = float( ABuffer.GetSampleRate() ) / FOutputSampleRate;
                    uint32_t AOutBitsPerSample = GetBytesPerSample() * 8;
                    uint32_t AInBitsPerSample = ABuffer.GetBitsPerSample();
                    uint32_t AOutBytesPerSample = GetBytesPerSample();
                    for( int i = 0; i < ABuffer.GetSize() * ARatio; ++ i )
                    {
                        uint8_t AData[ 4 ];
    					TAudioBuffer::CopySample( ABaseInPtr + MitovMin<uint32_t>( ABuffer.GetSize() - 1, i * AReverseRatio ) * AInStep, AData, AInStep, AOutStep, AInChannelsCount, FCountChannels(), ABytesPerSample, AOutBytesPerSample, AInBitsPerSample, AOutBitsPerSample, AZeroSize );
//                        Serial.println( *(int16_t *)AData );
                        SendToBufffer( FOutputRingBuffer, AData, AOutBytesPerSample, AOutBytesPerSample );
                    }
                }
            }
        }

        inline constexpr i2s_port_t GetStaticPortNumber()
        {
#if( SOC_I2S_NUM > 1 )
            return ( C_INDEX ) ? I2S_NUM_1 : I2S_NUM_0;
#else
            return I2S_NUM_0;
#endif
        }

    public:
		inline void SystemInit()
		{
//		    USBSerial.begin( 115200 );
//            while( ! USBSerial )
//              ;

//		    delay( 10000 );

//		    USBSerial.println( "SystemInit" );

            auto ALock = FMutex.GetLock();

            i2s_config_t AInputConfig = {};
            i2s_config_t AOutputConfig = {};
            bool ARatePopulated = false;
            bool ABitsPopulated = false;
            bool AModePopulated = false;
		    if( ! SampleRate().AutoConfig().GetValue() )
		    {
                AInputConfig.sample_rate = SampleRate().Rate();
                AOutputConfig.sample_rate = SampleRate().Rate();

                ARatePopulated = true;
		    }

		    if( ! Bits().AutoConfig().GetValue() )
		    {
                AInputConfig.bits_per_sample = i2s_bits_per_sample_t( Bits().Count().GetValue() );
                AOutputConfig.bits_per_sample = i2s_bits_per_sample_t( Bits().Count().GetValue() );

                ABitsPopulated = true;
		    }

	        if( IsSlave() )
	        {
	            AInputConfig.mode = I2S_MODE_SLAVE;
	            AOutputConfig.mode = I2S_MODE_SLAVE;
            }

	        else
	        {
	            AInputConfig.mode = I2S_MODE_MASTER;
	            AOutputConfig.mode = I2S_MODE_MASTER;
	        }

		    if( ! Mode().AutoConfig().GetValue() )
		    {
	            if( Mode().Mode() == TArduinoBasicI2S_ModeEnum::PDM )
	            {
	                AInputConfig.mode = i2s_mode_t( AInputConfig.mode | I2S_MODE_TX | I2S_MODE_PDM );
	                AInputConfig.communication_format = I2S_COMM_FORMAT_STAND_I2S;
	            }

	            else
	            {
	                AInputConfig.mode = i2s_mode_t( AInputConfig.mode | I2S_MODE_TX );
	                switch( Mode().Mode() )
	                {
                        case TArduinoBasicI2S_ModeEnum::Standard :   // Phillips
	                        AInputConfig.communication_format = I2S_COMM_FORMAT_STAND_I2S;
	                        break;

                        case TArduinoBasicI2S_ModeEnum::MSB :
	                        AInputConfig.communication_format = I2S_COMM_FORMAT_STAND_MSB;
	                        break;

                        case TArduinoBasicI2S_ModeEnum::ShortPCM :
	                        AInputConfig.communication_format = I2S_COMM_FORMAT_STAND_PCM_SHORT;
	                        break;

                        case TArduinoBasicI2S_ModeEnum::LongPCM :
	                        AInputConfig.communication_format = I2S_COMM_FORMAT_STAND_PCM_LONG;
	                        break;

//                            case TArduinoBasicI2S_ModeEnum::TDM :        // Multichannel
//		                        AInputConfig.communication_format = I2S_COMM_FORMAT_STAND_I2S;
//		                        break;

	                }
		        }

                AOutputConfig.mode = AInputConfig.mode;
                AOutputConfig.communication_format = AInputConfig.communication_format;

		        AModePopulated = true;
		    }

		    TI2SInfo AInfo;

            TI2SMode::TI2SMode AFormats[] =
            {
                TI2SMode::Standard,
                TI2SMode::MSB,
                TI2SMode::ShortPCM,
                TI2SMode::LongPCM,
                TI2SMode::PDM,
                TI2SMode::TDM
            };

            AInfo.Input.Modes.Resize( MITOV_ARRAY_SIZE( AFormats ) );
            AInfo.Input.Used = false;

//            Serial.println( AInfo.Input.Modes.size() );

            AInfo.Output.Modes.Resize( MITOV_ARRAY_SIZE( AFormats ) );
            AInfo.Output.Used = false;

//            USBSerial.println( "TEST2" );

            for( int i = 0; i < MITOV_ARRAY_SIZE( AFormats ); ++ i )
            {
    		    AInfo.Input.Modes[ i ].Mode = AFormats[ i ];
    		    AInfo.Input.Modes[ i ].MinSampleRate = 8000;
    		    AInfo.Input.Modes[ i ].MaxSampleRate = 96000; //192000;
    		    AInfo.Input.Modes[ i ].ChannelsMap = 0;
    		    AInfo.Input.Modes[ i ].Enabled = true;

    		    AInfo.Output.Modes[ i ].Mode = AFormats[ i ];
    		    AInfo.Output.Modes[ i ].MinSampleRate = 8000;
    		    AInfo.Output.Modes[ i ].MaxSampleRate = 96000; //192000;
    		    AInfo.Output.Modes[ i ].ChannelsMap = 0;
    		    AInfo.Output.Modes[ i ].Enabled = true;

    		    if( AFormats[ i ] == TI2SMode::PDM )
    		    {
        		    AInfo.Input.Modes[ i ].BitsPerSample = 16;
        		    AInfo.Output.Modes[ i ].BitsPerSample = 16;
    		    }

    		    else
    		    {
        		    AInfo.Input.Modes[ i ].BitsPerSample = 8;
        		    AInfo.Output.Modes[ i ].BitsPerSample = 8;
    		    }
            }

//            USBSerial.println( "TEST3" );
            T_ControlOutputPin::PinNotify( &AInfo );

            for( uint8_t i = 0; i < FControlCallbacks.size(); ++ i )
                FControlCallbacks[ i ].Callback( FControlCallbacks[ i ].Instance, &AInfo );

//            USBSerial.println( "TEST3_1_1" );

            if( ( ! AInfo.Input.Used ) && ( ! AInfo.Output.Used ) )
                return;

//            USBSerial.println( "TEST4" );
//            delay( 1000 );

            TI2SDirectionInfo AMergedInfo;
            AMergedInfo.Modes.Resize( MITOV_ARRAY_SIZE( AFormats ) );

            if( AInfo.Input.Used && AInfo.Output.Used ) // Merge the configs
            {
                for( int i = 0; i < MITOV_ARRAY_SIZE( AFormats ); ++ i )
                {
//                    USBSerial.println( i );
//                    delay( 1000 );

//                    USBSerial.println( AInfo.Input.Modes[ i ].Enabled );
//                    delay( 1000 );

//                    USBSerial.println( AInfo.Input.Modes[ i ].Enabled );
//                    delay( 1000 );

                    AMergedInfo.Modes[ i ].Enabled = AInfo.Input.Modes[ i ].Enabled && AInfo.Output.Modes[ i ].Enabled;
//                    USBSerial.println( "Step 1" );
//                    delay( 1000 );
                    if( ! AMergedInfo.Modes[ i ].Enabled )
                        continue;

//                    USBSerial.println( "Step 2" );
//                    delay( 1000 );

                    AMergedInfo.Modes[ i ].MinSampleRate = MitovMax( AInfo.Input.Modes[ i ].MinSampleRate, AInfo.Output.Modes[ i ].MinSampleRate );
                    AMergedInfo.Modes[ i ].MaxSampleRate = MitovMin( AInfo.Input.Modes[ i ].MaxSampleRate, AInfo.Output.Modes[ i ].MaxSampleRate );

                    AMergedInfo.Modes[ i ].BitsPerSample = MitovMax( AInfo.Input.Modes[ i ].BitsPerSample, AInfo.Output.Modes[ i ].BitsPerSample );

/*
                    if( ( AInfo.Input.Modes[ i ].MinBitsPerSample != 0 ) && ( AInfo.Output.Modes[ i ].MinBitsPerSample != 0 ) )
                    {
                        AMergedInfo.Modes[ i ].MinBitsPerSample = MitovMax( AInfo.Input.Modes[ i ].MinBitsPerSample, AInfo.Output.Modes[ i ].MinBitsPerSample );
                        AMergedInfo.Modes[ i ].MaxBitsPerSample = MitovMin( AInfo.Input.Modes[ i ].MaxBitsPerSample, AInfo.Output.Modes[ i ].MaxBitsPerSample );
                    }

                    else if( AInfo.Input.Modes[ i ].MinBitsPerSample != 0 )
                    {
                        for( int j = 0; j < AInfo.Output.Modes[ i ].BitsPerSample.size() ; ++ j )
                            if( ( AInfo.Output.Modes[ i ].BitsPerSample[ j ] >= AInfo.Input.Modes[ i ].MinBitsPerSample ) && ( AInfo.Output.Modes[ i ].BitsPerSample[ j ] <= AInfo.Input.Modes[ i ].MaxBitsPerSample ) )
                                AMergedInfo.Modes[ i ].BitsPerSample.push_back( AInfo.Output.Modes[ i ].BitsPerSample[ j ] );

                    }

                    else if( AInfo.Output.Modes[ i ].MinBitsPerSample != 0 )
                    {
                        for( int j = 0; j < AInfo.Input.Modes[ i ].BitsPerSample.size() ; ++ j )
                            if( ( AInfo.Input.Modes[ i ].BitsPerSample[ j ] >= AInfo.Output.Modes[ i ].MinBitsPerSample ) && ( AInfo.Input.Modes[ i ].BitsPerSample[ j ] <= AInfo.Output.Modes[ i ].MaxBitsPerSample ) )
                                AMergedInfo.Modes[ i ].BitsPerSample.push_back( AInfo.Input.Modes[ i ].BitsPerSample[ j ] );

                    }

                    else
                    {
                        for( int j = 0; j < AInfo.Input.Modes[ i ].BitsPerSample.size() ; ++ j )
                            for( int k = 0; k < AInfo.Output.Modes[ i ].BitsPerSample.size() ; ++ k )
                                if( AInfo.Input.Modes[ i ].BitsPerSample[ j ] == AInfo.Output.Modes[ i ].BitsPerSample[ k ] )
                                {
                                    AMergedInfo.Modes[ i ].BitsPerSample.push_back( AInfo.Input.Modes[ i ].BitsPerSample[ j ] );
                                    break;
                                }

                    }
*/
                    AMergedInfo.Modes[ i ].ChannelsMap = AInfo.Input.Modes[ i ].ChannelsMap | AInfo.Output.Modes[ i ].ChannelsMap;
                }
//                AInfo.Output. if( )

            }

//            USBSerial.println( "TEST5" );

//            Serial.println( "AOutputConfig.mode" );
//            Serial.println( AOutputConfig.mode );

//            delay( 1000 );

            AInputConfig.mode = i2s_mode_t( AInputConfig.mode | I2S_MODE_RX );
            AOutputConfig.mode = i2s_mode_t( AOutputConfig.mode | I2S_MODE_TX );

            FChannelMap = 0;
            if( AInfo.Input.Used && AInfo.Output.Used )
            {
                ConfigDirection( AMergedInfo, AInputConfig, AFormats, MITOV_ARRAY_SIZE( AFormats ), ARatePopulated, ABitsPopulated, AModePopulated, FChannelMap );

                FChannelMap = 0;
                ConfigDirection( AMergedInfo, AOutputConfig, AFormats, MITOV_ARRAY_SIZE( AFormats ), ARatePopulated, ABitsPopulated, AModePopulated, FChannelMap );
            }

            else
            {
                uint32_t AInputChannelMap = 0;
                uint32_t AOutputChannelMap = 0;

                ConfigDirection( AInfo.Input, AInputConfig, AFormats, MITOV_ARRAY_SIZE( AFormats ), ARatePopulated, ABitsPopulated, AModePopulated, AInputChannelMap );
                ConfigDirection( AInfo.Output, AOutputConfig, AFormats, MITOV_ARRAY_SIZE( AFormats ), ARatePopulated, ABitsPopulated, AModePopulated, AOutputChannelMap );

                if( AInfo.Input.Used )
                    FChannelMap = AInputChannelMap;

                else
                    FChannelMap = AOutputChannelMap;

            }

            switch( ( AInfo.Input.Used ) ? AInputConfig.bits_per_sample : AOutputConfig.bits_per_sample )
            {
                case I2S_BITS_PER_SAMPLE_8BIT : FBytesPerSample() = 0; break;
                case I2S_BITS_PER_SAMPLE_16BIT : FBytesPerSample() = 1; break;
                case I2S_BITS_PER_SAMPLE_24BIT : FBytesPerSample() = 2; break;

                default : 
                case I2S_BITS_PER_SAMPLE_32BIT : FBytesPerSample() = 3; break;
            }

//            USBSerial.println( GetBytesPerSample() );

            const int AInterruptLevels[]
            {
                ESP_INTR_FLAG_LEVEL1,
                ESP_INTR_FLAG_LEVEL2,
                ESP_INTR_FLAG_LEVEL3,
                ESP_INTR_FLAG_LEVEL4,
                ESP_INTR_FLAG_LEVEL5,
                ESP_INTR_FLAG_LEVEL6
            };

            AInputConfig.intr_alloc_flags = AInterruptLevels[ Input().InterruptLevel().GetValue() - 1 ];
            AInputConfig.dma_buf_count = Input().Buffers().Count();
            AInputConfig.dma_buf_len = Input().Buffers().Size();
            AOutputConfig.use_apll = false;

            AOutputConfig.intr_alloc_flags = AInterruptLevels[ Output().InterruptLevel().GetValue() - 1 ];
            AOutputConfig.dma_buf_count = Output().Buffers().Count();
            AOutputConfig.dma_buf_len = Output().Buffers().Size();
            AOutputConfig.use_apll = false;

            FInputBufferSize = AInputConfig.dma_buf_len * (( AInputConfig.bits_per_sample + 7 ) / 8 );
            FOutputBufferSize = AOutputConfig.dma_buf_len * (( AOutputConfig.bits_per_sample + 7 ) / 8 );

            FCountChannels() = 0;
            for( int i = 0; i < 32; ++ i )
                if( FChannelMap & ( 1 << i ) )
                    FCountChannels() = FCountChannels() + 1;


            int AInputBufferByteSize = FInputBufferSize * AInputConfig.dma_buf_count * FCountChannels().GetValue();
            int AOutputBufferByteSize = FOutputBufferSize * AOutputConfig.dma_buf_count * FCountChannels().GetValue();

            FInputBuffersCount() = AInputConfig.dma_buf_count;
            FOutputBuffersCount() = AOutputConfig.dma_buf_count;

            i2s_pin_config_t pin_config =
            {
                .mck_io_num = ( ExternalClocking().Enabled() ) ? ExternalClocking().PinNumber().GetValue() : 
#ifdef VISUINO_M5STACK_CORES3
                    0,
#else
                    I2S_PIN_NO_CHANGE,
#endif
                .bck_io_num = ClockPin().GetValue(),
                .ws_io_num = WordSelectPin().GetValue(),
                .data_out_num = DataOut().GetValue(),
                .data_in_num = DataIn().GetValue()
            };

//        Serial.println( pin_config.mck_io_num );  // -1
//        Serial.println( pin_config.bck_io_num );  // 12
//        Serial.println( pin_config.ws_io_num ); // 0
//        Serial.println( pin_config.data_out_num ); // 2
//        Serial.println( pin_config.data_in_num ); // 34

            FIsSecondChannel() = ( C_INDEX != 0 );

//            Serial.println( "TEST6_1" );

//            if( AOutputConfig.sample_rate > 44100 )
//                AOutputConfig.sample_rate = 44100;

//            AInputConfig.sample_rate = 44100;

            FOutputSampleRate = AOutputConfig.sample_rate;
            FInputSampleRate = AInputConfig.sample_rate;
/*
            USBSerial.print( "FOutputSampleRate: " );
            USBSerial.println( FOutputSampleRate );
            USBSerial.print( "FInputSampleRate: " );
            USBSerial.println( FInputSampleRate );
*/
            if( ! AInfo.Output.Used )
            {
//                FInputBuffersCount() = 0;
/*
                USBSerial.println( "TEST1" );
		        Serial.println( AInputBufferByteSize );
		        Serial.println( FInputBufferSize );
		        Serial.println( AInputConfig.dma_buf_count );
		        Serial.println( FCountChannels().GetValue() );
		        delay( 1000 );
*/
                FInputRingBuffer = xRingbufferCreate( AInputBufferByteSize, RINGBUF_TYPE_BYTEBUF );

//            Serial.println( "AInputConfig" );
//            Serial.println( AInputConfig.mode, BIN );

//            Serial.println( AInputConfig.sample_rate );

//            Serial.println( AInputConfig.intr_alloc_flags );
//            Serial.println( AInputConfig.dma_buf_count );
//            Serial.println( AInputConfig.dma_buf_len );

//            AInputConfig.sample_rate = 44100;

                i2s_driver_install( GetStaticPortNumber(), &AInputConfig, ESP32_I2S_Const::C_I2S_EVENT_QUEUE_LENGTH, &FEventQueue );
                i2s_set_pin( GetStaticPortNumber(), &pin_config );
                i2s_zero_dma_buffer( GetStaticPortNumber() );
                xTaskCreate(
//                    onDmaInputComplete,   // Function to implement the task
                    onDmaComplete,   // Function to implement the task
//                    "onDmaInputComplete", // Name of the task
                    "onDmaComplete", // Name of the task
                    20000,                   // Stack size in words
                    NULL,                    // Task input parameter
                    2,                       // Priority of the task
//                    &FCallbackTaskInputHandle     // Task handle.
                    &FCallbackTaskHandle     // Task handle.
                );

                i2s_set_clk( GetStaticPortNumber(), AInputConfig.sample_rate, AInputConfig.bits_per_sample, ( AInputConfig.channel_format == I2S_CHANNEL_FMT_RIGHT_LEFT ) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO );
//                i2s_set_clk( GetStaticPortNumber(), 44100, AInputConfig.bits_per_sample, ( AInputConfig.channel_format == I2S_CHANNEL_FMT_RIGHT_LEFT ) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO );
            }

            else if( ! AInfo.Input.Used )
            {
//                FOutputBuffersCount() = 0;
/*

                USBSerial.println( "TEST6" );
                USBSerial.println( FOutputBufferSize );
                USBSerial.println( AOutputConfig.dma_buf_count );
                USBSerial.println( FCountChannels().GetValue() );
                USBSerial.println( GetBytesPerSample() );
                USBSerial.print( "AOutputBufferByteSize: " );
                USBSerial.println( AOutputBufferByteSize );

                USBSerial.println( AOutputConfig.sample_rate );
                USBSerial.println( AOutputConfig.bits_per_sample );
		    delay( 1000 );
*/
                FOutputRingBuffer = xRingbufferCreate( AOutputBufferByteSize, RINGBUF_TYPE_BYTEBUF );

//                Serial.println( uint64_t( FOutputRingBuffer ));

//                auto ATest1 = xRingbufferGetCurFreeSize( FOutputRingBuffer );
//                Serial.println( ATest1 );

/*
AOutputConfig.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX );
AOutputConfig.sample_rate = 24000; //I2S_SAMPLE_RATE;
AOutputConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
AOutputConfig.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
AOutputConfig.communication_format = I2S_COMM_FORMAT_STAND_I2S;
AOutputConfig.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
AOutputConfig.dma_buf_count = 8;
AOutputConfig.dma_buf_len = 256;
AOutputConfig.use_apll = true;
AOutputConfig.tx_desc_auto_clear = false;
AOutputConfig.fixed_mclk = 0;
*/
/*
    AOutputConfig.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
//    AInputConfig.sample_rate = I2S_SAMPLE_RATE;
        AOutputConfig.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
        AOutputConfig.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
        AOutputConfig.communication_format = I2S_COMM_FORMAT_STAND_I2S;
        AOutputConfig.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
        AOutputConfig.dma_buf_count = 8;
        AOutputConfig.dma_buf_len = 256;
        AOutputConfig.use_apll = true;
        AOutputConfig.tx_desc_auto_clear = false;
        AOutputConfig.fixed_mclk = 0;
*/

//                i2s_driver_install( GetStaticPortNumber(), &AOutputConfig, ESP32_I2S_Const::C_I2S_EVENT_QUEUE_LENGTH, &FEventOutputQueue );
                i2s_driver_install( GetStaticPortNumber(), &AOutputConfig, ESP32_I2S_Const::C_I2S_EVENT_QUEUE_LENGTH, &FEventQueue );
                i2s_set_pin( GetStaticPortNumber(), &pin_config );
                i2s_zero_dma_buffer( GetStaticPortNumber() );

                xTaskCreate(
//                    onDmaOutputComplete,   // Function to implement the task
                    onDmaComplete,   // Function to implement the task
//                    "onDmaOutputComplete", // Name of the task
                    "onDmaComplete", // Name of the task
                    20000,                   // Stack size in words
                    NULL,                    // Task input parameter
                    2,                       // Priority of the task
//                    &FCallbackTaskOutputHandle     // Task handle.
                    &FCallbackTaskHandle     // Task handle.
                );

                i2s_set_clk( GetStaticPortNumber(), AOutputConfig.sample_rate, AOutputConfig.bits_per_sample, ( AOutputConfig.channel_format == I2S_CHANNEL_FMT_RIGHT_LEFT ) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO );

//                Serial.println( "TEST7" );
            }

            else
            {
//                USBSerial.println( "TEST3" );
//		        delay( 1000 );

				FIsSecondChannel() = false;
                FInputRingBuffer  = xRingbufferCreate( AInputBufferByteSize, RINGBUF_TYPE_BYTEBUF );
                FOutputRingBuffer = xRingbufferCreate( AOutputBufferByteSize, RINGBUF_TYPE_BYTEBUF );

//                USBSerial.println( "TEST3_1" );
/*
                USBSerial.println( AInputConfig.mode );
    USBSerial.println( AInputConfig.sample_rate );  
    USBSerial.println( AInputConfig.bits_per_sample );
    USBSerial.println( AInputConfig.channel_format ); 
    USBSerial.println( AInputConfig.communication_format );
    USBSerial.println( AInputConfig.intr_alloc_flags );
    USBSerial.println( AInputConfig.dma_buf_count );  
    USBSerial.println( AInputConfig.dma_buf_len );
    USBSerial.println( AInputConfig.use_apll );           
    USBSerial.println( AInputConfig.tx_desc_auto_clear ); 
    USBSerial.println( AInputConfig.fixed_mclk );         
    USBSerial.println( AInputConfig.mclk_multiple );      
    USBSerial.println( AInputConfig.bits_per_chan );      

//		        delay( 1000 );
*/
                AInputConfig.mode = i2s_mode_t( AInputConfig.mode | I2S_MODE_TX );
                i2s_driver_install( I2S_NUM_0, &AInputConfig, ESP32_I2S_Const::C_I2S_EVENT_QUEUE_LENGTH, &FEventQueue );
/*
                i2s_driver_install( I2S_NUM_0, &AInputConfig, ESP32_I2S_Const::C_I2S_EVENT_QUEUE_LENGTH, &FEventInputQueue );
                USBSerial.println( "TEST3_2" );
		        delay( 1000 );
                i2s_driver_install( I2S_NUM_1, &AOutputConfig, ESP32_I2S_Const::C_I2S_EVENT_QUEUE_LENGTH, &FEventOutputQueue );
*/
//                USBSerial.println( "TEST4" );
//                delay( 1000 );
                i2s_set_pin( I2S_NUM_0, &pin_config );
//                i2s_set_pin( I2S_NUM_1, &pin_config );

//                USBSerial.println( "TEST4_1" );
//                delay( 1000 );
                i2s_set_clk( I2S_NUM_0, AInputConfig.sample_rate, AInputConfig.bits_per_sample, ( AInputConfig.channel_format == I2S_CHANNEL_FMT_RIGHT_LEFT ) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO );
//                i2s_set_clk( I2S_NUM_0, AInputConfig.sample_rate, AInputConfig.bits_per_sample, ( AInputConfig.channel_format == I2S_CHANNEL_FMT_RIGHT_LEFT ) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO );
//                i2s_set_clk( I2S_NUM_1, AOutputConfig.sample_rate, AOutputConfig.bits_per_sample, ( AOutputConfig.channel_format == I2S_CHANNEL_FMT_RIGHT_LEFT ) ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO );
                i2s_zero_dma_buffer( I2S_NUM_0 );
                xTaskCreate(
//                    onDmaInputComplete,   // Function to implement the task
                    onDmaComplete,   // Function to implement the task
//                    "onDmaInputComplete", // Name of the task
                    "onDmaComplete", // Name of the task
                    20000,                   // Stack size in words
                    NULL,                    // Task input parameter
                    2,                       // Priority of the task
//                    &FCallbackTaskInputHandle     // Task handle.
                    &FCallbackTaskHandle     // Task handle.
                );

/*
                xTaskCreate(
                    onDmaOutputComplete,   // Function to implement the task
                    "onDmaOutputComplete", // Name of the task
                    20000,                   // Stack size in words
                    NULL,                    // Task input parameter
                    2,                       // Priority of the task
                    &FCallbackTaskOutputHandle     // Task handle.
                );
*/
            }

//return;
		}

	public:
        inline void SystemLoopBegin()
        {
//return;
            auto ALock = FMutex.GetLock();

            if( ! FInputRingBuffer )
                return;

//            Serial.print( "." );

            int ABufferByteSize = FInputBufferSize * FInputBuffersCount().GetValue() * FCountChannels().GetValue();

            int AAvailableSize = ABufferByteSize - (int)xRingbufferGetCurFreeSize( FInputRingBuffer );

//            Serial.print( "SystemLoopBegin: " );
//            Serial.println( AAvailableSize );

            uint32_t AChannelsCount = TAudioBuffer::GetChannelsCount( FChannelMap );

            int AReadSamples = AAvailableSize / GetBytesPerSample() / AChannelsCount;
            if( AReadSamples )
            {
                uint8_t *ABuffer = new uint8_t[ AReadSamples * GetBytesPerSample() * AChannelsCount ];
                size_t item_size = 0;
                void *tmp_buffer = xRingbufferReceiveUpTo( FInputRingBuffer, &item_size, pdMS_TO_TICKS(1000), AReadSamples * GetBytesPerSample() * AChannelsCount );
//                USBSerial.println( item_size );
                if( tmp_buffer )
                {
//                        Serial.println( item_size );
                    memcpy( ABuffer, tmp_buffer, item_size );
                    vRingbufferReturnItem( FInputRingBuffer, tmp_buffer );

                    TAudioBuffer AAudioBuffer( FChannelMap, uint8_t( GetBytesPerSample() * 8 ), FInputSampleRate, ( item_size / GetBytesPerSample() ) / AChannelsCount, ABuffer );
                    Input().OutputPin().SetPinValue( AAudioBuffer );

//                    Serial.print( "+" );
                    for( uint8_t i = 0; i < FOutputCallbacks.size(); ++ i )
                        FOutputCallbacks[ i ].Callback( FOutputCallbacks[ i ].Instance, &AAudioBuffer );


//                        FBytesPerSample
// FChannelMap


/*
                    #if (SOC_I2S_SUPPORTS_ADC && SOC_I2S_SUPPORTS_DAC)
                    if(_mode == ADC_DAC_MODE)
                    {
                        for(size_t i = 0; i < item_size / 2; ++i)
                            ((uint16_t*)buffer)[i] = ((uint16_t*)buffer)[i] & 0x0FFF;

                    } // ADC/DAC mode
                    #endif
*/
//                        return item_size;
                } // tmp buffer not NULL ?

                delete [] ABuffer;
            }
        }

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef USBSerial
#undef Serial
#endif