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
#include <Mitov_Structure.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	class TImageJPEGConvert_Null
	{
	public:
		inline static void Convert( const TImageBuffer &AImage, uint8_t *AData )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T_CONVERT
	> class PacketImageBinaryElement :
		public T_FPopulated
	{
		_V_PROP_( FPopulated )

	protected:
		TStoredImageBuffer	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TImageBuffer &AValue = *(TImageBuffer*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			if( FValue.GetSize() != AValue.GetSize() )
				C_OWNER.FNeedsNewSize() = true;

			FValue = AValue;
/*
            for( int i = 0; i < AValue.Width * AValue.Height / 2; ++i )
            {
                Serial.println( AValue.Read()[ i ], HEX );
            }
*/
			C_OWNER.FModified() = true;
		}

	protected:
//		inline void GetAddSize( int &AResult, uint32_t ALength )
//		{
//			do
//			{
//				++ AResult;
//				ALength >>= 7;
//			}
//			while( ALength );
//		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{
			AAlligned = true;

			AResult = 0;
			AResult += Func::GetVariableSize( FValue.GetWidth() );
			AResult += Func::GetVariableSize( FValue.GetHeight() );

//			Serial.println( AResult );

			if( FValue.GetPixelFormat() == TImageFormat::JPEG )
				AResult = ( AResult + ( ( FValue.GetWidth() * FValue.GetHeight() * 2 ) + 1 )) * 8; // RGB565
				
			else
				AResult = ( AResult + ( FValue.GetSize() + 1 )) * 8;
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			C_OWNER.GetAddVariableSizeValue( ADataStart, ADataIndex, FValue.GetWidth(), false );
			C_OWNER.GetAddVariableSizeValue( ADataStart, ADataIndex, FValue.GetHeight(), false );

			if( FValue.GetPixelFormat() == TImageFormat::JPEG )
			{
				ADataStart[ ADataIndex ++ ] = TImageFormat::RGB565;

				T_CONVERT::Convert( FValue, ADataStart + ADataIndex );
/*
//				uint32_t ALength = FValue.Width * FValue.Height;
				TUncompress AUncompressItem;
				AUncompressItem.Buffer = (uint16_t *)( ADataStart + ADataIndex );
//				AUncompressItem.Buffer = ADataStart + ADataIndex;
//				AUncompressItem.Buffer = new uint16_t[ ALength ];
				AUncompressItem.JPegImage = &FValue;

				Mitov_JPEGDEC::JPEGDEC *AJpeg = new Mitov_JPEGDEC::JPEGDEC;

//				Serial.println();
//				Serial.println( "Start" );
				AJpeg->openRAM( FValue.Read(), FValue.GetSize(), CompressImage );
				AJpeg->setUserPointer( & AUncompressItem );
//				AJpeg->setPixelType( RGB565_BIG_ENDIAN );
				AJpeg->setPixelType( Mitov_JPEGDEC::RGB565_LITTLE_ENDIAN );
				AJpeg->decode( 0, 0, 0 );

//				memcpy( ADataStart + ADataIndex, AUncompressItem.Buffer, ALength * 2 );

				AJpeg->close();
//				delete [] AUncompressItem.Buffer;

				delete AJpeg;
*/
				ADataIndex += FValue.GetWidth() * FValue.GetHeight() * 2;
			}

			else
			{
				ADataStart[ ADataIndex ++ ] = FValue.GetPixelFormat();

				uint32_t ALength = FValue.GetSize();
				memcpy( ADataStart + ADataIndex, FValue.Read(), ALength );

				ADataIndex += ALength;
			}

            FPopulated() = false;
/*
            FILE* pFile = fopen("D:\\PackBuffer1_3_C_1.bin", "wb");

            fwrite( ADataStart, 1, ADataIndex, pFile );

            fclose(pFile);
*/
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult )
		{
			AResult = false;
		}

	public:
		inline PacketImageBinaryElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif