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
	class TPacketAccumulator : public Mitov::SimpleList<uint8_t, uint16_t>
	{
		typedef Mitov::SimpleList<uint8_t, uint16_t> inherited;

	public:
		TPacketAccumulator & operator = ( const TPacketAccumulator &other )
		{
			assign( other );
			return *this;
		}

		void assign( const TPacketAccumulator &other )
		{
			inherited::assign( other );
		}

	public:
		bool SkipBytes( int ACount )
		{
			if( size() < ACount )
				return false;

//			Serial.print( "Size: " ); Serial.println( size() );
//			Serial.print( "SkipBytes: " ); Serial.println( ACount );
			pop_front( ACount );
			return true;
		}

		bool FindBytes( int ASize, const uint8_t *ABytesPtr, int AStartPos, int ASearchSize, int &APosition )
		{
			for( APosition = AStartPos; APosition < MitovMin( (int)( _size - ASize ), ASearchSize ); ++APosition )
				if( memcmp( ABytesPtr, inherited::_list + APosition, ASize ) == 0 )
				{
					APosition += ASize;
					return true;
				}

			return false;
		}

		bool FindBytes( int ASize, const uint8_t *ABytesPtr, int &APosition )
		{
			return FindBytes( ASize, ABytesPtr, 0, _size, APosition );
		}

		template <typename T_STORAGE> bool FindBytes( int AStartPos, int ASearchSize, int &APosition )
        {
            if( T_STORAGE::GetIsConstant() )
            {
                TValueArray<uint8_t> ABuffer( T_STORAGE::GetCount(), nullptr );
				uint8_t *AData = ABuffer.Write();
				T_STORAGE::CopyData( AData );
                return  FindBytes( T_STORAGE::GetCount(), AData, AStartPos, ASearchSize, APosition );
            }

            else
                return  FindBytes( T_STORAGE::GetCount(), T_STORAGE::GetArray(), AStartPos, ASearchSize, APosition );

        }

        template <typename T_STORAGE> void Append()
        {
            if( T_STORAGE::GetIsConstant() )
            {
                TValueArray<uint8_t> ABuffer( T_STORAGE::GetCount(), nullptr );
				uint8_t *AData = ABuffer.Write();
				T_STORAGE::CopyData( AData );
                append( AData, T_STORAGE::GetCount() );
            }

            else
                append( T_STORAGE::GetArray(), T_STORAGE::GetCount() );

        }

		bool ExtractBytes( int ASize, void *ABytesPtr )
		{
			if( size() < ASize )
				return false;

//			Serial.print( "Size: " ); Serial.println( size() );
//			Serial.print( "ExtractBytes: " ); Serial.println( ASize );

			memcpy( ABytesPtr, inherited::_list, ASize );
			pop_front( ASize );
			return true;
		}

		bool PeekBytes( int ASize, void *ABytesPtr )
		{
			if( size() < ASize )
				return false;

			memcpy( ABytesPtr, inherited::_list, ASize );
			return true;
		}

		bool PeekBytesFrom( int APosition, int ASize, void *ABytesPtr )
		{
			if( size() < APosition + ASize )
				return false;

			memcpy( ABytesPtr, inherited::_list + APosition, ASize );
			return true;
		}

	};
//---------------------------------------------------------------------------
	namespace PacketTypes
	{
		void IncreaseBuffer( uint8_t *&AOutBuffer, uint32_t &ACurrentSize, uint32_t ASize )
		{
			uint8_t *ABuffer = new uint8_t[ ACurrentSize + ASize ];
			memcpy( ABuffer, AOutBuffer, ACurrentSize );
			delete [] AOutBuffer;

			AOutBuffer = ABuffer;
			ACurrentSize += ASize;
		}
//---------------------------------------------------------------------------
        bool GetCanExtractSize( uint8_t *&AData, uint32_t &A_Size, bool &AResult, uint32_t &ASizeResult )
		{
			ASizeResult = 0;
			uint8_t AShift = 0;
			uint8_t AByteSize;
			do
			{
				if( ! A_Size )
				{
					AResult &= false;
					return false;
				}

				AByteSize = *AData ++;
				-- A_Size;

				ASizeResult += ( AByteSize & 0x7F ) << ( AShift * 7 );

				++ AShift;
			}
			while( AByteSize & 0x80 );

			return true;
        }
//---------------------------------------------------------------------------
	}
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T_PIN,
		typename T
	> class BasicTypedStructureSinkElement :
		public T_FPopulated
	{
	public:
		_V_PROP_( FPopulated )

	protected:
		T	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T AValue = *(T_PIN*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

#ifdef VISUINO_MEMORY_ALLIGN
			memcpy( ABuffer + ADataIndex, &FValue, sizeof( T ));
#else
			*((T*)( ABuffer + ADataIndex )) = FValue;
#endif
			ADataIndex += sizeof( T );
		}

	public:
		inline BasicTypedStructureSinkElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T_MostSignificantFirst,
		typename T_PIN,
		typename T
	> class BasicTypedOrderStructureSinkElement :
		public T_FPopulated,
		public T_MostSignificantFirst
	{
	public:
		_V_PROP_( MostSignificantFirst )

	public:
		_V_PROP_( FPopulated )

	protected:
		T	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T AValue = *(T_PIN*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			if( MostSignificantFirst() )
			{
				for( uint8_t i = 0; i < sizeof( T ); ++ i )
				{
					ABuffer[ ADataIndex ++ ] = FValue >> ( ( sizeof( T ) - 1 - i ) * 8 );
//					AValue <<= 8;
//					AValue |= ;
				}
			}

			else
			{
#ifdef VISUINO_MEMORY_ALLIGN
				memcpy( ABuffer + ADataIndex, &FValue, sizeof( T ));
#else
				*((T*)( ABuffer + ADataIndex )) = FValue;
#endif
				ADataIndex += sizeof( T );
			}

		}

	public:
		inline BasicTypedOrderStructureSinkElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T_MostSignificantFirst
	> class BasicTypedOrderStructureSinkFloatElement :
		public T_FPopulated,
		public T_MostSignificantFirst
	{
	public:
		_V_PROP_( MostSignificantFirst )

	public:
		_V_PROP_( FPopulated )

	protected:
		float	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			if( MostSignificantFirst() )
			{
				uint32_t ATmpValue = *(uint32_t *)&FValue;
				for( uint8_t i = 0; i < sizeof( float ); ++ i )
				{
					ABuffer[ ADataIndex ++ ] = ATmpValue >> ( ( sizeof( float ) - 1 - i ) * 8 );
//					AValue <<= 8;
//					AValue |= ;
				}
			}

			else
			{
#ifdef VISUINO_MEMORY_ALLIGN
				memcpy( ABuffer + ADataIndex, &FValue, sizeof( float ));
#else
				*((float*)( ABuffer + ADataIndex )) = FValue;
#endif
				ADataIndex += sizeof( float );
			}

		}

	public:
		inline BasicTypedOrderStructureSinkFloatElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_PIN,
		typename T
	> class BasicTypedStructureSourceElement :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ AData;
				AOffset = 0;
			}

			T_PIN AValue;
			AValue = *((T *)AData );
			T_OutputPin::SetPinValue( AValue );

			AData += sizeof( T );
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &ASize, uint8_t &AOffset, bool &AResult )
		{
			if( ! AResult )
				return;

			if( AOffset )
			{
				if( ! ASize )
				{
					AResult &= false;
					return;
				}

				-- ASize;
				++ AData;
				AOffset = 0;
			}

			if( ASize < sizeof( T ) )
			{
				AResult &= false;
				return;
			}

			ASize -= sizeof( T );
			AData += sizeof( T );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_MostSignificantFirst,
		typename T_OutputPin,
		typename T_PIN,
		typename T
	> class BasicTypedOrderStructureSourceElement :
		public T_MostSignificantFirst,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( MostSignificantFirst )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ AData;
				AOffset = 0;
			}

			T_PIN AValue;
			if( MostSignificantFirst() )
			{
				uint64_t ATmpValue = 0;
				for( uint8_t i = 0; i < sizeof( T ); ++i )
				{
					ATmpValue <<= 8;
					ATmpValue |= *AData ++;
				}

				AValue = *(T *)&ATmpValue;
			}

			else
			{
				AValue = *((T *)AData );
				AData += sizeof( T );
			}


			T_OutputPin::SetPinValue( AValue );

//			AData += sizeof( T );
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &ASize, uint8_t &AOffset, bool &AResult )
		{
			if( ! AResult )
				return;

			if( AOffset )
			{
				if( ! ASize )
				{
					AResult &= false;
					return;
				}

				-- ASize;
				++ AData;
				AOffset = 0;
			}

			if( ASize < sizeof( T ) )
			{
				AResult &= false;
				return;
			}

			ASize -= sizeof( T );
			AData += sizeof( T );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class DigitalStructureSourceElement :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			uint8_t AValue = *AData;
			bool ABoolValue = (( AValue & ( 1 << AOffset )) != 0 );
			++AOffset;

			if( AOffset >= 8 )
			{
				AOffset = 0;
				++AData;
			}

			T_OutputPin::SetPinValue( ABoolValue );
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &ASize, uint8_t &AOffset, bool &AResult )
		{
			if( ! AResult )
				return;

			++ AOffset;

			if( AOffset >= 8 )
			{
				AOffset = 0;
				if( ! ASize )
				{
					AResult &= false;
					return;
				}

				-- ASize;
				++ AData;
			}

			if( ! ASize )
			{
				AResult &= false;
				return;
			}
		}
	};
//---------------------------------------------------------------------------
	template <
        typename T_IdleOutputPin,
		typename T_OutputPin
	> class ClockStructureSourceElement :
        public T_IdleOutputPin,
		public T_OutputPin
	{
	public:
        _V_PIN_( IdleOutputPin )
		_V_PIN_( OutputPin )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			uint8_t AValue = *AData;
			bool ABoolValue = (( AValue & ( 1 << AOffset )) != 0 );
			++AOffset;

			if( AOffset >= 8 )
			{
				AOffset = 0;
				++AData;
			}

			if( ABoolValue )
				T_OutputPin::ClockPin();

			else
				T_IdleOutputPin::ClockPin();

//			T_OutputPin::SetPinValue( ABoolValue );
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &ASize, uint8_t &AOffset, bool &AResult )
		{
			if( ! AResult )
				return;

			++ AOffset;

			if( AOffset >= 8 )
			{
				AOffset = 0;
				if( ! ASize )
				{
					AResult &= false;
					return;
				}

				-- ASize;
				++ AData;
			}

			if( ! ASize )
			{
				AResult &= false;
				return;
			}
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class TextStructureSourceElement :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ AData;
				AOffset = 0;
			}

			uint32_t ASize = Func::GetVariableSizeValue( AData );

			uint8_t *ANewData = new uint8_t[ ASize + 1 ];

			memcpy( ANewData, AData, ASize );

			ANewData[ ASize ] = '\0';

			T_OutputPin::SetPinValue( (char *)ANewData );

			delete [] ANewData;

			AData += ASize;
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &A_Size, uint8_t &AOffset, bool &AResult )
		{
			if( ! AResult )
				return;

			if( AOffset )
			{
				if( ! A_Size )
				{
					AResult &= false;
					return;
				}

				-- A_Size;
				++ AData;
				AOffset = 0;
			}

			uint32_t ASize;
			if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, ASize ))
                return;

			if( A_Size < ASize )
			{
				AResult &= false;
				return;
			}

			A_Size -= ASize;
			AData += ASize;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T
	> class ArrayStructureSourceElement :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ AData;
				AOffset = 0;
			}

			uint32_t ASize = Func::GetVariableSizeValue( AData );

			Mitov::TArray<T> AOutData( ASize, (T *)AData );
			T_OutputPin::SetPinValue( AOutData );

			AData += ASize * sizeof( T );
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &A_Size, uint8_t &AOffset, bool &AResult )
		{
			if( AOffset )
			{
				if( ! A_Size )
				{
					AResult &= false;
					return;
				}

				-- A_Size;
				++ AData;
				AOffset = 0;
			}

			uint32_t ASize;
			if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, ASize ))
                return;

			ASize *= sizeof( T );
			if( A_Size < ASize )
			{
				AResult &= false;
				return;
			}

			A_Size -= ASize;
			AData += ASize;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class ImageStructureSourceElement :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ AData;
				AOffset = 0;
			}

			uint32_t AWidth = Func::GetVariableSizeValue( AData );
			uint32_t AHeight = Func::GetVariableSizeValue( AData );
			TImageFormat::TImageFormat APixelFormat = TImageFormat::TImageFormat( *AData ++ );
			if( APixelFormat == TImageFormat::JPEG )
			{
				uint32_t ASize = Func::GetVariableSizeValue( AData );
				TImageBuffer ABuffer( APixelFormat, AWidth, AHeight, AData, ASize );
				T_OutputPin::SetPinValue( ABuffer );
                AData += ABuffer.GetSize();
			}

			else
			{
				TImageBuffer ABuffer( APixelFormat, AWidth, AHeight, AData );
				T_OutputPin::SetPinValue( ABuffer );
                AData += ABuffer.GetSize();
			}
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &A_Size, uint8_t &AOffset, bool &AResult )
		{
			if( ! AResult )
				return;

			if( AOffset )
			{
				if( ! A_Size )
				{
					AResult &= false;
					return;
				}

				-- A_Size;
				++ AData;
				AOffset = 0;
			}

			uint32_t AWidth;
			if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, AWidth ))
				return;

			uint32_t AHeight;
			if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, AHeight ))
				return;

			// Skip format byte
			if( ! A_Size )
			{
				AResult &= false;
				return;
			}

			-- A_Size;
			TImageFormat::TImageFormat APixelFormat = TImageFormat::TImageFormat( *AData ++ );

			uint32_t ASize;

			if( APixelFormat == TImageFormat::JPEG )
			{
				if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, ASize ))
					return;

			}

			else
				ASize = TImageFormat::GetSize( APixelFormat, AWidth, AHeight );

			if( A_Size < ASize )
			{
				AResult &= false;
				return;
			}

			A_Size -= ASize;
			AData += ASize;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class AudioStructureSourceElement :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Extract( const uint8_t *& AData, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ AData;
				AOffset = 0;
			}

			uint32_t AChannelsMap = Func::GetVariableSizeValue( AData );
			uint8_t ABitsPerSample = *AData ++;
			uint32_t ASampleRate = Func::GetVariableSizeValue( AData );
			uint32_t ASize = Func::GetVariableSizeValue( AData );

			TAudioBuffer ABuffer( AChannelsMap, ABitsPerSample, ASampleRate, ASize, AData );
			T_OutputPin::SetPinValue( ABuffer );

			AData += ABuffer.GetSize();
		}

		inline void CanExtract( uint8_t *&AData, uint32_t &A_Size, uint8_t &AOffset, bool &AResult )
		{
			if( ! AResult )
				return;

			if( AOffset )
			{
				if( ! A_Size )
				{
					AResult &= false;
					return;
				}

				-- A_Size;
				++ AData;
				AOffset = 0;
			}

			uint32_t AChannelsMap;
			if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, AChannelsMap ))
				return;

			// Skip Bits Per Sample byte
			if( ! A_Size )
			{
				AResult &= false;
				return;
			}

			-- A_Size;
			uint8_t ABitsPerSample = *AData ++;

			uint32_t ASampleRate;
			if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, ASampleRate ))
				return;

			uint32_t ADataSize;
			if( ! PacketTypes::GetCanExtractSize( AData, A_Size, AResult, ADataSize ))
				return;

			uint32_t ASize = TAudioBuffer::GetSize( AChannelsMap, ABitsPerSample, ADataSize );
			if( A_Size < ASize )
			{
				AResult &= false;
				return;
			}

			A_Size -= ASize;
			AData += ASize;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T_FValue
	> class DigitalStructureSinkElement :
		public T_FPopulated,
		protected T_FValue
	{
	public:
		_V_PROP_( FPopulated )

	protected:
		_V_PROP_( FValue )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue() )
					return;

			FPopulated() = true;
			FValue() = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset == 0 )
				ABuffer[ ADataIndex ] = 0;

			ABuffer[ ADataIndex ] &= 0xFF >> ( 8 - AOffset ); // Zero the upper bits
			if( FValue() )
				ABuffer[ ADataIndex ] |= 1 << AOffset;	// Set the bit

			++ AOffset;
			if( AOffset >= 8 )
			{
				AOffset = 0;
				++ ADataIndex;
			}
		}

	public:
		inline DigitalStructureSinkElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_BufferSize,
		typename T_FCount
	> class ClockStructureSinkElement :
		public T_BufferSize,
		protected T_FCount
	{
	public:
		_V_PROP_( BufferSize )

	protected:
		_V_PROP_( FCount )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FCount().GetValue() >= BufferSize().GetValue() )
				return;

			FCount() = FCount() + 1;

			C_OWNER.FRefreshed() = true;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
//			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset == 0 )
				ABuffer[ ADataIndex ] = 0;

			ABuffer[ ADataIndex ] &= 0xFF >> ( 8 - AOffset ); // Zero the upper bits
			if( FCount() )
			{
				FCount() = FCount() - 1;
				ABuffer[ ADataIndex ] |= 1 << AOffset;	// Set the bit

				if( FCount() )
				{
					C_OWNER.FRefreshed() = true;
					C_OWNER.FModified() = true;
				}
			}

			++ AOffset;
			if( AOffset >= 8 )
			{
				AOffset = 0;
				++ ADataIndex;
			}
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated
	> class TextStructureSinkElement :
		public T_FPopulated
	{
	public:
		_V_PROP_( FPopulated )

	protected:
		Mitov::String	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AValue = (char*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ADataIndex;
				AOffset = 0;
			}

			uint32_t ALength = FValue.length();

			PacketTypes::IncreaseBuffer( ABuffer, ACurrentSize, ALength );

    	    uint8_t ACount = Func::GetAddVariableSizeValue( ABuffer, ADataIndex, ALength );

			if( ACount > 1 )
				PacketTypes::IncreaseBuffer( ABuffer, ACurrentSize, ACount - 1 );

			memcpy( ABuffer + ADataIndex, FValue.c_str(), ALength );
			ADataIndex += ALength;
		}

	public:
		inline TextStructureSinkElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated
	> class ImageStructureSinkElement :
		public T_FPopulated
	{
	public:
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

			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ADataIndex;
				AOffset = 0;
			}

            uint32_t AWidth = FValue.GetWidth();
            uint32_t AHeight = FValue.GetHeight();

//    	    uint8_t ACount = Func::GetAddVariableSizeValue( ABuffer, ADataIndex, AWidth );
    	    uint8_t ACount = Func::GetVariableSize( AWidth );
    	    ACount += Func::GetVariableSize( AHeight );
//   	        uint32_t AWidthIndex = ADataIndex;

//    	    ACount += Func::GetAddVariableSizeValue( ABuffer, ADataIndex, AHeight );
//   	        uint32_t AHeightIndex = ADataIndex;

			uint32_t ALength = FValue.GetSize();
			PacketTypes::IncreaseBuffer( ABuffer, ACurrentSize, ALength + ACount - 2 );  // 2 bytes are already prereserved


            Func::GetAddVariableSizeValue( ABuffer, ADataIndex, AWidth );
            Func::GetAddVariableSizeValue( ABuffer, ADataIndex, AHeight );

			ABuffer[ ADataIndex ++ ] = FValue.GetPixelFormat();

			if( FValue.GetPixelFormat() == TImageFormat::JPEG )
	            Func::GetAddVariableSizeValue( ABuffer, ADataIndex, FValue.GetSize() );

			memcpy( ABuffer + ADataIndex, FValue.Read(), ALength );

			ADataIndex += ALength;
		}

	public:
		inline ImageStructureSinkElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated
	> class AudioStructureSinkElement :
		public T_FPopulated
	{
	public:
		_V_PROP_( FPopulated )

	protected:
		TStoredAudioBuffer	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TAudioBuffer &AValue = *(TAudioBuffer*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ADataIndex;
				AOffset = 0;
			}

			uint32_t ASize = FValue.GetSize();
    	    uint8_t ACount = Func::GetVariableSize( ASize );
    	    uint8_t ASampleRateCount = Func::GetVariableSize( FValue.GetSampleRate() );

			uint32_t ALength = FValue.GetSize();
			PacketTypes::IncreaseBuffer( ABuffer, ACurrentSize, ALength + ASampleRateCount + ACount - 1 );  // 1 byte is already prereserved

            Func::GetAddVariableSizeValue( ABuffer, ADataIndex, FValue.GetChannelsMap() );
			ABuffer[ ADataIndex ++ ] = FValue.GetBitsPerSample();

			Func::GetAddVariableSizeValue( ABuffer, ADataIndex, FValue.GetSampleRate() );

			Func::GetAddVariableSizeValue( ABuffer, ADataIndex, ASize );

			memcpy( ABuffer + ADataIndex, FValue.Read(), ALength );

			ADataIndex += ALength;
		}

	public:
		inline AudioStructureSinkElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T
	> class ArrayStructureSinkElement :
		public T_FPopulated
	{
	public:
		_V_PROP_( FPopulated )

	protected:
		TValueArray<T>	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TArray<T> &AValue = *(TArray<T>*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( AOffset )
			{
				++ADataIndex;
				AOffset = 0;
			}

			uint32_t ALength = FValue.GetSize();

			PacketTypes::IncreaseBuffer( ABuffer, ACurrentSize, ALength * sizeof( T ) );

    	    uint8_t ACount = Func::GetAddVariableSizeValue( ABuffer, ADataIndex, ALength );

			if( ACount > 1 )
				PacketTypes::IncreaseBuffer( ABuffer, ACurrentSize, ACount - 1 );

			ALength *= sizeof( T );
			memcpy( ABuffer + ADataIndex, FValue.Read(), ALength );
			ADataIndex += ALength;
		}

	public:
		inline ArrayStructureSinkElement()
		{
			FPopulated() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		int T_BUFFER_SIZE,
		uint32_t C_COUNT,
		typename T_Elements_CheckPopulated,
		typename T_Elements_GetData,
		typename T_FModified,
		typename T_FRefreshed,
		typename T_OnlyModified,
		typename T_OutputPin
	> class MakeStructure :
		public T_FModified,
		public T_FRefreshed,
		public T_OnlyModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
//		Mitov::SimpleObjectList<BasicMakeStructureElement*>	Elements;

		_V_PROP_( OnlyModified )

	public:
		_V_PROP_( FModified )
		_V_PROP_( FRefreshed )

//	protected:
//		uint8_t FBuffer[ T_BUFFER_SIZE ];

	protected:
		bool IsPopulated()
		{
			bool AResult = true;
			T_Elements_CheckPopulated::Call( AResult );
			return AResult;
		}

		void GetData( uint8_t *&ABuffer, uint32_t &ACurrentSize, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			T_Elements_GetData::Call( ABuffer, ACurrentSize, ADataIndex, AOffset );
//			PacketTypes::TGetStructDataOperation AOperation( ABuffer, ACurrentSize, ADataIndex, AOffset );
//			ProcessCallChain( &AOperation );
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "++++++" );
//			int AElementCount = Elements.size();
			if( OnlyModified() )
			{
				if( ! FModified() )
					return;
			}

			else
			{
				if( ! IsPopulated() )
					return;
//				for( int i = 0; i < C_COUNT; ++i )
//					if( ! IsPopulated( i ) )
//						return;

			}

			uint32_t ACurrentSize = T_BUFFER_SIZE;
			uint8_t *ABuffer = new uint8_t[ T_BUFFER_SIZE ];
			uint32_t ADataIndex = 0;

//			uint8_t *ADataPtr = ABuffer;
			uint8_t AOffset = 0;
//			Serial.println( ACurrentSize );
//			for( int i = 0; i < C_COUNT; ++i )
			FModified() = false;
			FRefreshed() = false;

			GetData( ABuffer, ACurrentSize, ADataIndex, AOffset );
//				Elements[ i ]->GetData( ADataPtr, AOffset );

//			Serial.println( ASize );
//			Serial.println( ACurrentSize );


			T_OutputPin::SendPinValue( Mitov::TDataBlock( ACurrentSize, ABuffer ));

			delete [] ABuffer;

/*
			ADataPtr = FBuffer;
			int ASize = T_BUFFER_SIZE;

			while( ASize-- )
				OutputPin.Notify( ADataPtr++ );
*/
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FRefreshed() )
				ClockInputPin_o_Receive( nullptr );

		}
/*
		inline void SystemStart()
		{
			FBufferSize = 0;
			FAllElements.push_back( &HeadMarker );
			for( int i = 0; i < Elements.size(); ++i )
				FAllElements.push_back( Elements[ i ] );

			FAllElements.push_back( &Checksum );

			for( int i = 0; i < FAllElements.size(); ++i )
			{
				bool AAlligned = false;
				FBufferSize += FAllElements[ i ]->GetSize( AAlligned );
//				Serial.println( FBufferSize );
				if( AAlligned )
				{
					FBufferSize = ( FBufferSize + 7 ) / 8;
					FBufferSize *= 8;
				}

			}

//			Serial.println( FBufferSize );

			FBufferSize = ( FBufferSize + 7 ) / 8;
			FBuffers[ 0 ] = new uint8_t[ FBufferSize * 2 ];
			FBuffers[ 1 ] = new uint8_t[ FBufferSize * 2 ];
		}
*/

	public:
		MakeStructure()
		{
			FModified() = false;
			FRefreshed() = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		int T_BUFFER_SIZE,
		uint32_t C_COUNT,
		typename T_Elements_CanExtract,
		typename T_Elements_Extract,
		typename T_FIndex,
		typename T_UnpackedOutputPin
	> class SplitStructure :
		public T_FIndex,
		public T_UnpackedOutputPin
	{
	public:
		_V_PIN_( UnpackedOutputPin )

	protected:
		uint8_t FBuffer[ T_BUFFER_SIZE ];
		_V_PROP_( FIndex );

	protected:
/*
		inline bool CanExtract( uint8_t *&AData, uint32_t &ASize, uint8_t &AOffset )
		{
			bool AResult = true;
			T_Elements_CanExtract::Call( AData, ASize, AOffset, AResult );
			return AResult;
		}
*/
		inline void Extract( const uint8_t *&AData, uint8_t &AOffset )
		{
			T_Elements_Extract::Call( AData, AOffset );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;
			uint32_t ABlockSize = ABlock.GetSize();
			const uint8_t *ABlockData = ABlock.Read();
			while( ABlockSize -- )
			{
//			    *FDataPtr ++ = *ABlockData ++;
				FBuffer[ FIndex() ] = *ABlockData ++;
				FIndex() = FIndex() + 1; // Do not use ++
//				uint32_t ASize = ( FDataPtr - FBuffer );
				if( FIndex() >= T_BUFFER_SIZE )
				{
					uint8_t AOffset = 0;
					const uint8_t *ADataPtr = FBuffer;
					Extract( ADataPtr, AOffset );

					T_UnpackedOutputPin::ClockPin();

//					FDataPtr = FBuffer;
					FIndex() = 0;
				}
			}
		}

	public:
		SplitStructure()
		{
			FIndex() = 0;
		}
	};
//---------------------------------------------------------------------------
	template <
		int T_BUFFER_SIZE,
		uint32_t C_COUNT,
		typename T_Elements_CanExtract,
		typename T_Elements_Extract,
		typename T_FIndex,
		typename T_UnpackedOutputPin
	> class SplitStructure_Variable :
		public T_FIndex,
		public T_UnpackedOutputPin
	{
	public:
		_V_PIN_( UnpackedOutputPin )

	protected:
		TPacketAccumulator	FAccumulator;
//		uint8_t *FBuffer; // [ T_BUFFER_SIZE ];
//		uint8_t *FDataPtr = FBuffer;

	protected:
		inline bool CanExtract( uint8_t *&AData, uint32_t &ASize, uint8_t &AOffset )
		{
			bool AResult = true;
			T_Elements_CanExtract::Call( AData, ASize, AOffset, AResult );
			return AResult;
		}

		inline void Extract( const uint8_t *&AData, uint8_t &AOffset )
		{
			T_Elements_Extract::Call( AData, AOffset );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			{ // Use local variable to reduce memory usage
				Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;
//				Serial.println( ABlock.ToString() );
				FAccumulator.append( ABlock.Read(), ABlock.GetSize() );
			}

			uint32_t ASize = FAccumulator.size();
//			Serial.print( "ASize = " );
//			Serial.println( ASize );
			if( ASize >= T_BUFFER_SIZE )
			{
				uint8_t AOffset = 0;
				uint8_t *ADataPtr = FAccumulator;

				if( ! CanExtract( ADataPtr, ASize, AOffset ))
					return;
/*
				for( int i = 0; i < C_COUNT; ++i )
					if( ! CanExtract( i, ADataPtr, ASize, AOffset ))
//					{
//						Serial.print( "Cancel = " );
//						Serial.println( i );
						return;
//					}
*/

				uint32_t ASubtractSize = FAccumulator.size() - ASize;

				AOffset = 0;
				ADataPtr = FAccumulator;
				Extract( ADataPtr, AOffset );

				FAccumulator.pop_front( ASubtractSize );
				T_UnpackedOutputPin::ClockPin();
			}
		}

/*
		void SystemStart()
		{
			FDataPtr = FBuffer;
		}
*/
	};
//---------------------------------------------------------------------------

}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif