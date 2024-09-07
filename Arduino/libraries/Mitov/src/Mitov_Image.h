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

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
        typename T_DataOutputPin,
		typename T_Enabled,
		typename T_FormatOutputPin,
		typename T_HeightOutputPin,
        typename T_WidthOutputPin
	> class ExtractImageInfo :
        public T_DataOutputPin,
		public T_Enabled,
		public T_FormatOutputPin,
		public T_HeightOutputPin,
        public T_WidthOutputPin
	{
	public:
		_V_PIN_( DataOutputPin )
		_V_PIN_( FormatOutputPin )
		_V_PIN_( HeightOutputPin )
		_V_PIN_( WidthOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			TImageBuffer &AValue = *(TImageBuffer*)_Data;

//            Serial.println( AValue.GetSize() );

            T_HeightOutputPin::SetPinValue( AValue.GetHeight() );
			T_WidthOutputPin::SetPinValue( AValue.GetWidth() );
            T_FormatOutputPin::SetPinValue( AValue.GetPixelFormat() );

            if( T_DataOutputPin::GetPinIsConnected() )
                T_DataOutputPin::SetPinValue( Mitov::TDataBlock( AValue.GetSize(), AValue.Read() ) );

        }

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_IncludeFormat,
        typename T_OutputPin
    > class ImageToBinary :
		public T_Enabled,
        public T_IncludeFormat,
        public T_OutputPin
    {
	public:
        _V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
        _V_PROP_( IncludeFormat )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			TImageBuffer &ABuffer = *(TImageBuffer*)_Data;

            const uint8_t *AInBufferData = ABuffer.Read();

            uint32_t ASize = ABuffer.GetSize();
            if( ! IncludeFormat().GetValue() )
                T_OutputPin::SetPinValue( Mitov::TDataBlock( ASize, AInBufferData ) );

            else
            {
                uint8_t AOffset = 1;

				AOffset += Func::GetVariableSize( ABuffer.GetWidth() );
                AOffset += Func::GetVariableSize( ABuffer.GetHeight() );

			    if( ABuffer.GetPixelFormat() == TImageFormat::JPEG )
                    AOffset += Func::GetVariableSize( ABuffer.GetHeight() );

                uint8_t *ABufferData = new uint8_t[ ASize + AOffset ];

                ABufferData[ 0 ] = ABuffer.GetPixelFormat();

                uint32_t ADataIndex = 1;

				Func::GetAddVariableSizeValue( ABufferData, ADataIndex, ABuffer.GetWidth() );
				Func::GetAddVariableSizeValue( ABufferData, ADataIndex, ABuffer.GetHeight() );
			    if( ABuffer.GetPixelFormat() == TImageFormat::JPEG )
            		Func::GetAddVariableSizeValue( ABufferData, ADataIndex, ASize );

                memcpy( ABufferData + AOffset, AInBufferData, ASize );
                ASize += AOffset;

                T_OutputPin::SetPinValue( Mitov::TDataBlock( ASize, ABufferData ) );

                delete [] ABufferData;
            }
        }

    };
//---------------------------------------------------------------------------
	template <
        typename T_ExtractFromData,
        typename T_Height,
        typename T_PixelFormat,
        typename T_Size,
        typename T_Width
    > class TArduinoBinaryToImageFormat :
        public T_ExtractFromData,
        public T_Height,
        public T_PixelFormat,
        public T_Size,
        public T_Width
    {
	public:
		_V_PROP_( ExtractFromData )
		_V_PROP_( Height )
		_V_PROP_( PixelFormat )
		_V_PROP_( Size )
		_V_PROP_( Width )

    };
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
        typename T_Format,
        typename T_OutputPin
    > class BinaryToImage :
		public T_Enabled,
        public T_Format,
        public T_OutputPin
    {
	public:
        _V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
        _V_PROP_( Format )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			TArray<uint8_t> &ABuffer = *(TArray<uint8_t>*)_Data;

            TImageFormat::TImageFormat AFormat;
            uint32_t AWidth;
            uint32_t AHeight;
            uint8_t *ABufferData = nullptr;
            const uint8_t *AData;

            int32_t ABufferSize = ABuffer.GetSize();
            const uint8_t *AInBufferData = ABuffer.Read();
            if( Format().ExtractFromData() )
            {
//                AData = ABuffer.Data;

                if( ABufferSize < 3 )
                    return; // Not enogh data

                AFormat = TImageFormat::TImageFormat( AInBufferData[ 0 ] );
                if( AFormat > TImageFormat::MaxFormat )
                    return; // Incorrect image format

                uint32_t ADataIndex = 1;
                AWidth = Func::GetVariableSizeValue( AInBufferData, ABufferSize, ADataIndex );
                AHeight = Func::GetVariableSizeValue( AInBufferData, ABufferSize, ADataIndex );
                if( AWidth == 0 || AHeight == 0 )
                    return; // Incorrect image size

                uint32_t ASize;
			    if( AFormat == TImageFormat::JPEG )
                    ASize = Func::GetVariableSizeValue( AInBufferData, ABufferSize, ADataIndex );

                else
                    ASize = TImageFormat::GetSize( AFormat, AWidth, AHeight );

                if( ASize + ADataIndex <= ABufferSize )
                    AData = AInBufferData + ADataIndex;

                else
                {
                    ABufferData = new uint8_t[ ASize ];
                    memcpy( ABufferData, AInBufferData + ADataIndex, ABufferSize - ADataIndex );
                    memset( ABufferData + ABufferSize - ADataIndex, 0, ASize - ( ABufferSize - ADataIndex ));
                    AData = ABufferData;
                }

            }

            else
            {
//                Serial.println( "STEP1" );
                AFormat = Format().PixelFormat();
                AWidth = Format().Width();
                AHeight = Format().Height();
                uint32_t ASize;
                if( AFormat == TImageFormat::JPEG )
                    ASize = Format().Size();

                else
                    ASize = TImageFormat::GetSize( AFormat, AWidth, AHeight );

//                Serial.println( ASize );
//                Serial.println( ABuffer.Size );
                if( ASize <= ABufferSize )
                    AData = AInBufferData;

                else
                {
                    ABufferData = new uint8_t[ ASize ];
                    AData = ABufferData;
                    memcpy( ABufferData, AInBufferData, ABufferSize );
                    memset( ABufferData + ABufferSize, 0, ASize - ABufferSize );
                }
            }

            T_OutputPin::SetPinValue( TImageBuffer( AFormat, AWidth, AHeight, AData ) );

            if( ABufferData )
                delete [] ABufferData;

        }

    };
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

