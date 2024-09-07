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
#include <Mitov_ImageBuffer.h>
#include <Mitov_JPEGDEC.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	class TStoredImageBufferHelperJPEG : public TStoredImageBufferHelper
	{
		typedef TStoredImageBufferHelper inherited;

	public:
		struct TUncompress
		{
			uint16_t *Buffer;
			const TImageBuffer *JPegImage;
		};

	public:
		static int CompressImage( Mitov_JPEGDEC::JPEGDRAW *pDraw )
		{
			uint16_t *AData = pDraw->pPixels;

			TUncompress *AUncompress = (TUncompress *)pDraw->pUser;
			for( uint32_t y = pDraw->y; y < pDraw->y + pDraw->iHeight; ++ y )
			{
				uint16_t *AStart = AUncompress->Buffer + ( y * pDraw->iWidth + pDraw->x );
				for( uint32_t x = 0; x < pDraw->iWidth; ++ x )
					*AStart++ = *AData++;

			}

			return 1; // returning true (1) tells JPEGDEC to continue decoding. Returning false (0) would quit decoding immediately.
		}

	public:
		using inherited::inherited;

		TStoredImageBufferHelperJPEG( const TImageBuffer &AOther, TImageFormat::TImageFormat APixelFormat ) :
			inherited( AOther, APixelFormat )
		{
			if( AOther.GetPixelFormat() != TImageFormat::JPEG )
				return;

			Mitov_JPEGDEC::JPEGDEC *AJpeg = new Mitov_JPEGDEC::JPEGDEC;
			AJpeg->openRAM( AOther.Read(), AOther.GetSize(), CompressImage );

			TUncompress AUncompressItem;
			AUncompressItem.JPegImage = &AOther;

			AJpeg->setUserPointer( & AUncompressItem );
//							AJpeg->setPixelType( Mitov_JPEGDEC::RGB565_LITTLE_ENDIAN );
			AJpeg->setPixelType( Mitov_JPEGDEC::RGB565_BIG_ENDIAN );

			if( APixelFormat == TImageFormat::RGB565 )
			{
				AUncompressItem.Buffer = ( uint16_t *)inherited::Write();
				AJpeg->decode( 0, 0, 0 );
			}

			else
			{
				TStoredImageBuffer ATmpBuffer( TImageFormat::RGB565, AOther.GetWidth(), AOther.GetHeight() );
				AUncompressItem.Buffer = ( uint16_t *)ATmpBuffer.Write();
				AJpeg->decode( 0, 0, 0 );
				TStoredImageBufferHelper::PopulateBuffer( *this, ATmpBuffer );
			}

			AJpeg->close();
			delete AJpeg;
		}

	};
//---------------------------------------------------------------------------
	class TImageJPEGConvert
	{
	public:
		inline static void Convert( const TImageBuffer &AImage, uint8_t *AData )
		{
//				uint32_t ALength = FValue.Width * FValue.Height;
			TStoredImageBufferHelperJPEG::TUncompress AUncompressItem;
			AUncompressItem.Buffer = (uint16_t *)AData;
//				AUncompressItem.Buffer = ADataStart + ADataIndex;
//				AUncompressItem.Buffer = new uint16_t[ ALength ];
			AUncompressItem.JPegImage = &AImage;

			Mitov_JPEGDEC::JPEGDEC *AJpeg = new Mitov_JPEGDEC::JPEGDEC;

//				Serial.println();
//				Serial.println( "Start" );
			AJpeg->openRAM( AImage.Read(), AImage.GetSize(), TStoredImageBufferHelperJPEG::CompressImage );
			AJpeg->setUserPointer( & AUncompressItem );
//				AJpeg->setPixelType( RGB565_BIG_ENDIAN );
			AJpeg->setPixelType( Mitov_JPEGDEC::RGB565_LITTLE_ENDIAN );
			AJpeg->decode( 0, 0, 0 );

//				memcpy( ADataStart + ADataIndex, AUncompressItem.Buffer, ALength * 2 );

			AJpeg->close();
//				delete [] AUncompressItem.Buffer;

			delete AJpeg;
		}

	};
//---------------------------------------------------------------------------
} // Mitov

