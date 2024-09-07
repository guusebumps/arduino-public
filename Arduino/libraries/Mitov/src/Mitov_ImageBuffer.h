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

namespace Mitov
{
//---------------------------------------------------------------------------
	class TStoredImageBufferHelper : public TStoredImageBuffer
	{
		typedef TStoredImageBuffer inherited;

	public:
		static void PopulateBuffer( TImageBuffer &ASelf, const TImageBuffer &AOther )
		{
			if( ASelf.GetPixelFormat() == AOther.GetPixelFormat() )
			{
				ASelf = AOther;
				return;
			}

			uint32_t AInStride = AOther.GetStride();
			uint32_t AOutStride = ASelf.GetStride();

			const uint8_t *AInPtrBase = AOther.Read();
			uint8_t *AOutPtrBase = ASelf.Write();

			switch( ASelf.GetPixelFormat() )
			{
				case TImageFormat::Gray4 :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::GrayScale :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth() / 2; ++ x )
								{
									*AOutPtr ++ = ( AInPtr[ 1 ] & 0xF0 ) | ( AInPtr[ 0 ] >> 4 );
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::Gray16 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth() / 2; ++ x )
								{
									*AOutPtr ++ = ( AInPtr[ 2 ] & 0xF0 ) | ( AInPtr[ 0 ] >> 4 );
									AInPtr += 4;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB24 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth() / 2; ++ x )
								{
									*AOutPtr ++ = ( Func::ColorToGrayscale( AInPtr[ 3 ], AInPtr[ 4 ], AInPtr[ 5 ] ) & 0xF0 ) | ( Func::ColorToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ], AInPtr[ 2 ] ) >> 4 );
									AInPtr += 6;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGBA32 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth() / 2; ++ x )
								{
									*AOutPtr ++ = ( Func::ColorToGrayscale( AInPtr[ 5 ], AInPtr[ 6 ], AInPtr[ 7 ] ) & 0xF0 ) | ( Func::ColorToGrayscale( AInPtr[ 1 ], AInPtr[ 2 ], AInPtr[ 3 ] ) >> 4 );
									AInPtr += 8;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB565 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth() / 2; ++ x )
								{
									*AOutPtr ++ = ( Func::Color565ToGrayscale( AInPtr[ 2 ], AInPtr[ 3 ] ) & 0xF0 ) | ( Func::Color565ToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ] ) >> 4 );
									AInPtr += 4;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB555 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth() / 2; ++ x )
								{
									*AOutPtr ++ = ( Func::Color555ToGrayscale( AInPtr[ 2 ], AInPtr[ 3 ] ) & 0xF0 ) | ( Func::Color555ToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ] ) >> 4 );
									AInPtr += 4;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::JPEG :
							break;

					}

					break;

				case TImageFormat::GrayScale :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::Gray4 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									if( x & 1 )
									{
										*AOutPtr ++ = *AInPtr & 0xF0;
										++ AInPtr;
									}

									else
										*AOutPtr ++ = *AInPtr << 4;

								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::Gray16 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = *AInPtr;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB24 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = Func::ColorToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ], AInPtr[ 2 ] );
									AInPtr += 3;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGBA32 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = Func::ColorToGrayscale( AInPtr[ 1 ], AInPtr[ 2 ], AInPtr[ 3 ] );
									AInPtr += 4;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB565 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = Func::Color565ToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ] );
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB555 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
	//									*AOutPtr ++ = Func::Color555ToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ] );
									*AOutPtr ++ = Func::Color555ToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ] );
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::JPEG :
							break;

					}

					break;

				case TImageFormat::Gray16 :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::Gray4 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									if( x & 1 )
									{
										*AOutPtr ++ = *AInPtr & 0xF0;
										++ AInPtr;
									}

									else
										*AOutPtr ++ = *AInPtr << 4;

									*AOutPtr ++ = 0;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::GrayScale :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = *AInPtr ++;
									*AOutPtr ++ = 0;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB24 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = Func::ColorToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ], AInPtr[ 2 ] );
									*AOutPtr ++ = 0;
									AInPtr += 3;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGBA32 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = Func::ColorToGrayscale( AInPtr[ 1 ], AInPtr[ 2 ], AInPtr[ 3 ] );
									*AOutPtr ++ = 0;
									AInPtr += 4;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB565 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = Func::Color565ToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ] );
									*AOutPtr ++ = 0;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB555 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = Func::Color555ToGrayscale( AInPtr[ 0 ], AInPtr[ 1 ] );
									*AOutPtr ++ = 0;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::JPEG :
							break;

					}

					break;

				case TImageFormat::RGB24 :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::Gray4 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									if( x & 1 )
									{
										*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr & 0xF0;
										++ AInPtr;
									}

									else
										*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr << 4;

								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::GrayScale :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
									*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr ++;

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::Gray16 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGBA32 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									++ AInPtr;
									*AOutPtr ++ = *AInPtr ++;
									*AOutPtr ++ = *AInPtr ++;
									*AOutPtr ++ = *AInPtr ++;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB565 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = AInPtr[ 0 ] & 0xf8;
									*AOutPtr ++ = ( ( AInPtr[ 0 ] & 0x07 ) << 5 ) | ( ( AInPtr[ 1 ] & 0xE0 ) >> 3 );
									*AOutPtr ++ = ( AInPtr[ 1 ] & 0x1F ) << 3;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB555 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = AInPtr[ 0 ] & 0xf8;
									*AOutPtr ++ = ( ( AInPtr[ 0 ] & 0x03 ) << 6 ) | ( ( AInPtr[ 1 ] & 0xE0 ) >> 2 );
									*AOutPtr ++ = ( AInPtr[ 1 ] & 0x1F ) << 3;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::JPEG :
							break;

					}

					break;

				case TImageFormat::RGBA32 :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::Gray4 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = 0xFF;
									if( x & 1 )
									{
										*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr & 0xF0;
										++ AInPtr;
									}

									else
										*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr << 4;

								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::GrayScale :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = 0xFF;
									*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr ++;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::Gray16 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = 0xFF;
									*AOutPtr ++ = *AOutPtr ++ = *AOutPtr ++ = *AInPtr;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB24 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = 0xFF;
									*AOutPtr ++ = *AInPtr ++;
									*AOutPtr ++ = *AInPtr ++;
									*AOutPtr ++ = *AInPtr ++;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB565 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = 0xFF;
									*AOutPtr ++ = AInPtr[ 0 ] & 0xf8;
									*AOutPtr ++ = ( ( AInPtr[ 0 ] & 0x07 ) << 5 ) | ( ( AInPtr[ 1 ] & 0xE0 ) >> 3 );
									*AOutPtr ++ = ( AInPtr[ 1 ] & 0x1F ) << 3;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB555 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = 0xFF;
									*AOutPtr ++ = AInPtr[ 0 ] & 0xf8;
									*AOutPtr ++ = ( ( AInPtr[ 0 ] & 0x03 ) << 6 ) | ( ( AInPtr[ 1 ] & 0xE0 ) >> 2 );
									*AOutPtr ++ = ( AInPtr[ 1 ] & 0x1F ) << 3;
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::JPEG :
							break;

					}

					break;

				case TImageFormat::RGB565 :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::Gray4 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									uint8_t AValue;
									if( x & 1 )
									{
										// (uint16_t(r & 0xF8) << 8) | (uint16_t(g & 0xFC) << 3) | (b >> 3)
										AValue = *AInPtr & 0xF0;
										++ AInPtr;
									}

									else
										AValue = *AInPtr << 4;

									*AOutPtr ++ = ( AValue & 0xF8 ) | (( AValue & 0xFC ) >> 5 );
									*AOutPtr ++ = (( AValue & 0xFC ) << 3 ) | ( AValue >> 3 );
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::GrayScale :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = ( *AInPtr & 0xF8 ) | (( *AInPtr & 0xFC ) >> 5 );
									*AOutPtr ++ = (( *AInPtr & 0xFC ) << 3 ) | ( *AInPtr >> 3 );
									++ AInPtr;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::Gray16 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = ( *AInPtr & 0xF8 ) | (( *AInPtr & 0xFC ) >> 5 );
									*AOutPtr ++ = (( *AInPtr & 0xFC ) << 3 ) | ( *AInPtr >> 3 );
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB24 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = ( AInPtr[ 0 ] & 0xF8 ) | (( AInPtr[ 1 ] & 0xFC ) >> 5 );
									*AOutPtr ++ = (( AInPtr[ 1 ] & 0xFC ) << 3 ) | ( AInPtr[ 2 ] >> 3 );
									AInPtr += 3;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGBA32 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = ( AInPtr[ 1 ] & 0xF8 ) | (( AInPtr[ 2 ] & 0xFC ) >> 5 );
									*AOutPtr ++ = (( AInPtr[ 2 ] & 0xFC ) << 3 ) | ( AInPtr[ 3 ] >> 3 );
									AInPtr += 4;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB555 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = ( AInPtr[ 0 ] << 1 ) | ( AInPtr[ 1 ] >> 7 );
									*AOutPtr ++ = (( AInPtr[ 1 ] << 1 ) & 0b11000000 ) | ( AInPtr[ 1 ] & 0b00011111 );
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::JPEG :
							break;

					}

					break;

				case TImageFormat::RGB555 :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::Gray4 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									uint8_t AValue;
									if( x & 1 )
									{
										// (uint16_t(r & 0xF8) << 8) | (uint16_t(g & 0xFC) << 3) | (b >> 3)
										AValue = *AInPtr & 0xF0;
										++ AInPtr;
									}

									else
										AValue = *AInPtr << 4;

									*AOutPtr ++ = ( AValue >> 1 ) | ( AValue >> 6 );
									*AOutPtr ++ = ( AValue << 2 ) | ( AValue >> 3 );
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::GrayScale :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = (( *AInPtr & 0xF8 ) >> 1 ) | (( *AInPtr & 0xF8 ) >> 6 );
									*AOutPtr ++ = (( *AInPtr & 0xF8 ) << 2 ) | ( *AInPtr >> 3 );
									++ AInPtr;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::Gray16 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = (( *AInPtr & 0xF8 ) >> 1 ) | (( *AInPtr & 0xF8 ) >> 6 );
									*AOutPtr ++ = (( *AInPtr & 0xF8 ) << 2 ) | ( *AInPtr >> 3 );
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB24 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = (( AInPtr[ 0 ] & 0xF8 ) >> 1 ) | (( AInPtr[ 1 ] & 0xF8 ) >> 6 );
									*AOutPtr ++ = (( AInPtr[ 1 ] & 0xF8 ) << 2 ) | ( AInPtr[ 2 ] >> 3 );
									AInPtr += 3;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGBA32 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = (( AInPtr[ 1 ] & 0xF8 ) >> 1 ) | (( AInPtr[ 2 ] & 0xF8 ) >> 6 );
									*AOutPtr ++ = (( AInPtr[ 2 ] & 0xF8 ) << 2 ) | ( AInPtr[ 3 ] >> 3 );
									AInPtr += 4;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::RGB565 :
							for( int y = 0; y < ASelf.GetHeight(); ++ y )
							{
								const uint8_t *AInPtr = AInPtrBase;
								uint8_t *AOutPtr = AOutPtrBase;
								for( int x = 0; x < ASelf.GetWidth(); ++ x )
								{
									*AOutPtr ++ = AInPtr[ 0 ] >> 1;
									*AOutPtr ++ = ( AInPtr[ 0 ] << 7 ) | (( AInPtr[ 1 ] >> 1 ) & 0b11100000 ) | ( AInPtr[ 1 ] & 0b00011111 );
									AInPtr += 2;
								}

								AInPtrBase += AInStride;
								AOutPtrBase += AOutStride;
							}

							break;

						case TImageFormat::JPEG :
							break;

					}

					break;

				case TImageFormat::JPEG :
					switch( AOther.GetPixelFormat() )
					{
						case TImageFormat::Gray4 :
							break;

						case TImageFormat::GrayScale :
							break;

						case TImageFormat::Gray16 :
							break;

						case TImageFormat::RGB24 :
							break;

						case TImageFormat::RGBA32 :
							break;

						case TImageFormat::RGB565 :
							break;

						case TImageFormat::RGB555 :
							break;

					}

					break;

			}
		}

	public:
		using inherited::inherited;

		TStoredImageBufferHelper( const TImageBuffer &AOther, TImageFormat::TImageFormat APixelFormat ) :
			inherited( APixelFormat, AOther.GetWidth(), AOther.GetHeight() )
		{
			PopulateBuffer( *this, AOther );
		}

	};
//---------------------------------------------------------------------------
} // Mitov

