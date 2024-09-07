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
		typename T_DATA,
		typename T_DIFFERENCE,
		typename T_DifferenceOutputPin,
		typename T_Enabled,
		typename T_FStarted,
		typename T_OutputPin,
		typename T_Threshold
	> class ChangeOnlyThreshold :
		public T_DifferenceOutputPin,
		public T_Enabled,
		public T_Threshold,
		public T_OutputPin,
		public T_FStarted
	{
	public:
		_V_PIN_( DifferenceOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Threshold )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FStarted )
		T_DATA FData;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T_DATA AValue = *(T_DATA*)_Data;
			if( Enabled() )
				if( FStarted() )
					if( abs( FData - AValue ) <= Threshold() )
						return;

			if( FStarted() )
			{
				T_DIFFERENCE ADifference = AValue - FData;
				T_DifferenceOutputPin::SetPinValue( ADifference );
			}

			else
			{
				FStarted() = true;
				T_DifferenceOutputPin::PinNotify( _Data );
			}

			FData = AValue;
			T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_DifferenceOutputPin,
		typename T_Enabled,
		typename T_FStarted,
		typename T_OutputPin,
		typename T_Threshold
	> class ComplexChangeOnlyThreshold :
		public T_DifferenceOutputPin,
		public T_Enabled,
		public T_Threshold,
		public T_OutputPin,
		public T_FStarted
	{
	public:
		_V_PIN_( DifferenceOutputPin )
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Threshold )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FStarted )
		TComplex FData;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TComplex AValue = *(TComplex*)_Data;
			if( Enabled() )
				if( FStarted() )
					if( abs( FData.Real - AValue.Real ) <= Threshold().GetValue().Real )
						if( abs( FData.Imaginary - AValue.Imaginary ) <= Threshold().GetValue().Imaginary )
							return;

			FStarted() = true;
			TComplex ADifference = AValue - FData;
			FData = AValue;
			T_OutputPin::PinNotify( _Data );
			T_DifferenceOutputPin::SetPinValue( ADifference );
        }

    };
//---------------------------------------------------------------------------
	template <
		typename T_DATA,
		typename T_Enabled,
		typename T_FStarted,
		typename T_OutputPin
	> class ChangeOnly :
		public T_Enabled,
		public T_OutputPin,
		public T_FStarted
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FStarted );
		T_DATA FData;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T_DATA AValue = *(T_DATA*)_Data;
			if( Enabled() )
				if( FStarted() )
					if( FData == AValue )
						return;

			FStarted() = true;
			FData = AValue;
			T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_DATA,
		typename T_Enabled,
		typename T_FStarted,
		typename T_OutputPin
	> class ChangeOnlyArray :
		public T_Enabled,
		public T_OutputPin,
		public T_FStarted
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FStarted );
		TValueArray<T_DATA> FData;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			::Mitov::TArray<T_DATA> &AValue = *( ::Mitov::TArray<T_DATA> *)_Data;
			if( Enabled() )
				if( FStarted() )
					if( FData == AValue )
						return;

			FStarted() = true;
			FData = AValue;
			T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FStarted,
		typename T_OutputPin
	> class ChangeOnlyText :
		public T_Enabled,
		public T_OutputPin,
		public T_FStarted
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FStarted );
		String FData;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			String AValue = (char*)_Data;
			if( Enabled() )
				if( FStarted() )
					if( FData == AValue )
						return;

			FStarted() = true;
			FData = AValue;
			T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FData,
		typename T_FStarted,
		typename T_OutputPin
	> class ChangeOnlyDigital :
		public T_Enabled,
		public T_OutputPin,
		public T_FStarted,
		public T_FData
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FStarted )
		_V_PROP_( FData )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool*)_Data;
			if( Enabled() )
				if( FStarted() )
					if( FData() == AValue )
						return;

			FStarted() = true;
			FData() = AValue;
			T_OutputPin::PinNotify( _Data );
		}

	public:
		ChangeOnlyDigital()
		{
			FData() = false;
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif