////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2024 Mitov Software. All Rights Reserved.        //
//                                                                            //
//     Developed by Swellington Santos                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _SWELLINGTON_MPL3115A2_H_
#define _SWELLINGTON_MPL3115A2_H_

#pragma once

#include <Wire.h>
#include <Mitov.h>

namespace Mitov
{

    //---------------------------------------------------------------------------
    enum MPL311A2_REG
    {
        STATUS = 0x00,
        OUT_P_MSB = 0x01,
        OUT_T_MSB = 0x04,
        PT_DATA_CFG = 0x13,
        BAR_IN_MSB = 0x14,
        CTRL_REG1 = 0x26
    };
    //---------------------------------------------------------------------------
    template <
        typename T_BaseLinePressure,
        typename T_Enabled,
        typename T_InFeet,
        typename T_OutputPin>
    class TArduinoMPL3115A2Altitude : 
        public T_BaseLinePressure,
        public T_Enabled,
        public T_InFeet,
        public T_OutputPin
    {
    public:
        _V_PIN_(OutputPin)

    public:
        _V_PROP_(BaseLinePressure)
        _V_PROP_(Enabled)
        _V_PROP_(InFeet)
    };
    //---------------------------------------------------------------------------
    template <
        typename T_Enabled,
        typename T_OutputPin>
    class TArduinoMPL3115A2Pressure : 
        public T_Enabled,
        public T_OutputPin
    {
    public:
        _V_PIN_(OutputPin)

    public:
        _V_PROP_(Enabled)
    };
    //---------------------------------------------------------------------------
    template <
        typename T_Enabled,
        typename T_InFahrenheit,
        typename T_OutputPin>
    class TArduinoMPL3115A2Temperature : 
        public T_Enabled,
        public T_InFahrenheit,
        public T_OutputPin
    {
    public:
        _V_PIN_(OutputPin)

    public:
        _V_PROP_(Enabled)
        _V_PROP_(InFahrenheit)
    };
    //---------------------------------------------------------------------------
    template <
        typename T_I2C, T_I2C &C_I2C,
        typename T_Address,
        typename T_Altitude,
        typename T_Enabled,
        typename T_Oversampling,
        typename T_Pressure,
        typename T_Temperature>
    class MPL3115A2_Sensor : 
        public T_Address,
        public T_Altitude,
        public T_Enabled,
        public T_Oversampling,
        public T_Pressure,
        public T_Temperature
    {
    public:
        _V_PROP_(Address)
        _V_PROP_(Altitude)
        _V_PROP_(Enabled)
        _V_PROP_(Oversampling)
        _V_PROP_(Pressure)
        _V_PROP_(Temperature)

    public:
        inline void Update_BaseLinePressure() const // Used For Live Bindings - Keep public!
        {
            Write16(MPL311A2_REG::BAR_IN_MSB, uint16_t(Altitude().BaseLinePressure().GetValue()));
        }

        void UpdateREG1() const // Used For Live Bindings - Keep public!
        {
            uint8_t AValue = GetOrderSample(Oversampling());
            AValue <<= 3;
            if (Altitude().OutputPin().GetPinIsConnected())
                AValue |= (1 << 7); // Set in Altitude Mode

//            AValue &= ~(1 << 1);
            Write8( MPL311A2_REG::CTRL_REG1, AValue & 0b11111110 ); // standby

//            Write8(MPL311A2_REG::CTRL_REG1, AValue);

            Write8( MPL311A2_REG::CTRL_REG1, AValue | 0b00000001 ); // active
        };

    private:
        inline uint8_t GetOrderSample(uint8_t AValue) const
        {
            if (AValue >= 128)
                return 0x07;

            if (AValue >= 64)
                return 0x06;

            if (AValue >= 32)
                return 0x05;

            if (AValue >= 16)
                return 0x04;

            if (AValue >= 8)
                return 0x03;

            if (AValue >= 4)
                return 0x02;

            if (AValue >= 2)
                return 0x01;

            return 0;
        };

        void WriteReg(uint8_t ARegister) const
        {
            C_I2C.beginTransmission(uint8_t(Address()));
            C_I2C.write(ARegister);
            C_I2C.endTransmission(false);
        };

        void Write8(uint8_t ARegister, uint8_t AValue) const
        {
            C_I2C.beginTransmission(uint8_t(Address()));
            C_I2C.write(ARegister);
            C_I2C.write(AValue);
            C_I2C.endTransmission();
        };

        void Write16(uint8_t ARegister, uint16_t AValue) const
        {
            C_I2C.beginTransmission(uint8_t(Address()));
            C_I2C.write(ARegister);
            C_I2C.write(uint8_t(AValue >> 8));
            C_I2C.write(uint8_t(AValue & 0xFF));
            C_I2C.endTransmission();
        };

        bool Read8(uint8_t ARegister, uint8_t *buffer, uint8_t len) const
        {
            C_I2C.beginTransmission(uint8_t(Address()));
            C_I2C.write(ARegister);
            C_I2C.endTransmission(false);
            C_I2C.requestFrom(uint8_t(Address()), _VISUINO_I2C_SIZE_(len));

            if (C_I2C.available() == len)
            {
                for (uint8_t i = 0; i < len; i++)
                    buffer[i] = C_I2C.read();

                return true;
            }

            return false;
        };

        void ReadTemperature()
        {
            if (!Temperature().OutputPin().GetPinIsConnected())
                return;

            uint8_t _buffer[5] = {0};

            Read8(MPL311A2_REG::STATUS, _buffer, 1);

            if (_buffer[0] & (1 << 1) == 0)
                UpdateREG1();

            if (Read8(MPL311A2_REG::OUT_T_MSB, _buffer, 2))
            {
                uint8_t msb = _buffer[0];
                uint8_t lsb = _buffer[1];
                uint16_t foo = 0;
                bool negSign = false;
                if (msb > 0x7f)
                {
                    foo = ~((msb << 8) + lsb) + 1;
                    msb = foo >> 8;
                    lsb = foo & 0x00f0;
                    negSign = true;
                }

                float templsb = (lsb >> 4) / 16.0;
                float temperature = float(msb + templsb);
                if (negSign)
                    temperature = 0 - temperature;

                float temp = Temperature().InFahrenheit() ? Mitov::Func::ConvertCtoF(temperature) : temperature;
                Temperature().OutputPin().SetPinValue(temp);
            }
        };

        void ReadPressure() const
        {
            uint8_t _buffer[3] = {0};

            Read8(MPL311A2_REG::STATUS, _buffer, 1);

            if (_buffer[0] & (1 << 2) == 0)
                UpdateREG1();

            if (Read8(MPL311A2_REG::OUT_P_MSB, _buffer, 3))
            {
                uint8_t msb = _buffer[0];
                uint8_t csb = _buffer[1];
                uint8_t lsb = _buffer[2];

                if (Altitude().OutputPin().GetPinIsConnected())
                {
                    float altitude;
                    if (msb > 0x7F)
                    {
                        long ATmp = ~(long(msb) << 16 | long(csb) << 8 | lsb) + 1; // 2's complement the data
                        altitude = float(ATmp >> 8) + float((lsb >> 4) / 16.0);    // Whole number plus fraction altitude in meters for negative altitude
                        altitude = -altitude;
                    }

                    else
                        altitude = (float)((msb << 16) | csb ) + (float)((lsb >> 4) / 24.0);//here

                    float APressure = (BaseLinePressure()) * pow((1 - altitude / 44330.77), 1.0 / 0.1902632);
                    altitude = Altitude().InFeet() ? altitude * 3.28084 : altitude;
                    Altitude().OutputPin().SetPinValue(altitude);
                    Pressure().OutputPin().SetPinValue( APressure * 100 );
                }

                else if (Pressure().OutputPin().GetPinIsConnected())
                {
                    long pressure_whole = (long)msb << 16 | (long)csb << 8 | (long)lsb;
                    pressure_whole >>= 6;
                    lsb &= 0b00110000;
                    lsb >>= 4;
                    float pressure_decimal = (float)lsb / 4.0;
                    float APressure = (float)pressure_whole + pressure_decimal;
                    Pressure().OutputPin().SetPinValue(APressure);
                }
            }
        };

        void enableEventFlags() const
        {
            Write8(MPL311A2_REG::PT_DATA_CFG, 0x07);
        };

        void Reset() const
        {
            Write8( MPL311A2_REG::CTRL_REG1, 0b00000100 ); // Reset
            delay( 1 );
            UpdateREG1();
            Update_BaseLinePressure();
            enableEventFlags();
        }

    public:
        inline void ResetInputPin_o_Receive(void *_data) const
        {
            Reset();
        }

        inline void ClockInputPin_o_Receive(void *_data) const
        {
            ReadTemperature();
            ReadPressure();
        };

    public:
        void SystemInit() const
        {
            Reset();
        };

        inline void SystemLoopBegin() const
        {
            ClockInputPin_o_Receive(nullptr);
        };
    };

}

#endif