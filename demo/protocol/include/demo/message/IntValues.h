//
// Copyright 2016 - 2025 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

/// @file
/// @brief Contains definition of IntValues message and its fields.

#pragma once

#include "comms/fields.h"
#include "comms/MessageBase.h"
#include "demo/MsgId.h"
#include "demo/FieldBase.h"
#include "demo/DefaultOptions.h"

#include <type_traits>

namespace demo
{

namespace message
{

/// @brief Accumulates details of all the IntValues message fields.
/// @tparam TOpt Extra options
/// @see IntValues
template <typename TOpt = demo::DefaultOptions>
struct IntValuesFields
{
    /// @brief Simple 2 byte unsigned value.
    /// @details The valid values are in range [0, 10]
    class field1 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint16_t,
            typename TOpt::message::IntValuesFields::field1,
            comms::option::ValidNumValueRange<0, 10>,
            comms::option::def::HasName
        >
    {
        using Base = 
            comms::field::IntValue<
                demo::FieldBase,
                std::uint16_t,
                typename TOpt::message::IntValuesFields::field1,
                comms::option::ValidNumValueRange<0, 10>,
                comms::option::def::HasName
            >;
    public:
        using ValueType = typename Base::ValueType;
        using SpecialNameInfo = std::pair<ValueType, const char*>;
        using SpecialNamesMapInfo = std::pair<const SpecialNameInfo*, std::size_t>;

        static constexpr bool hasSpecials()
        {
            return true;
        }

        static const char* name()
        {
            return "field1";
        }   

        static SpecialNamesMapInfo specialNamesMap()
        {
            static const SpecialNameInfo Map[] = {
                std::make_pair(ValueType(1), "S1"),
                std::make_pair(ValueType(5), "S2"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }                      
    };

    /// @brief Signed integer serialised using only 3 bytes
    class field2 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::int32_t,
            typename TOpt::message::IntValuesFields::field2,
            comms::option::FixedLength<3>,
            comms::option::def::HasName
        >
    {
    public:
        static constexpr bool hasSpecials()
        {
            return false;
        }

        static const char* name()
        {
            return "field2";
        }                 
    };

    /// @brief Variable length (base-128) encoded unsigned integer value
    class field3 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint32_t,
            typename TOpt::message::IntValuesFields::field3,
            comms::option::VarLength<1, 4>,
            comms::option::def::HasName
        >
    {
        using Base =
            comms::field::IntValue<
                demo::FieldBase,
                std::uint32_t,
                typename TOpt::message::IntValuesFields::field3,
                comms::option::VarLength<1, 4>,
                comms::option::def::HasName
            >;
    public:
        using ValueType = typename Base::ValueType;
        using SpecialNameInfo = std::pair<ValueType, const char*>;
        using SpecialNamesMapInfo = std::pair<const SpecialNameInfo*, std::size_t>;

        static constexpr bool hasSpecials()
        {
            return true;
        }

        static const char* name()
        {
            return "field3";
        }   

        static SpecialNamesMapInfo specialNamesMap()
        {
            static const SpecialNameInfo Map[] = {
                std::make_pair(ValueType(100), "S1"),
                std::make_pair(ValueType(500), "S2"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }                        
    };

    /// @brief Example of serialising year information as a single byte.
    /// @details Serialised as offset from year 2000 using 1 byte.
    ///     Default constructed value is 2016
    class field4 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::int16_t,
            typename TOpt::message::IntValuesFields::field4,
            comms::option::FixedLength<1>,
            comms::option::NumValueSerOffset<-2000>,
            comms::option::DefaultNumValue<2016>,
            comms::option::ValidNumValueRange<2000, 2255>,
            comms::option::def::HasName
        >
    {
    public:
        static constexpr bool hasSpecials()
        {
            return false;
        }

        static const char* name()
        {
            return "field4";
        }                 
    };

    /// @brief Signed integer serialised using 6 bytes
    class field5 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::int64_t,
            typename TOpt::message::IntValuesFields::field5,
            comms::option::FixedLength<6>,
            comms::option::ValidNumValueRange<static_cast<std::int64_t>(0xffff800000000000), 0x7fffffffffff>,
            comms::option::def::HasName
        >
    {
        using Base =
        comms::field::IntValue<
            demo::FieldBase,
            std::int64_t,
            typename TOpt::message::IntValuesFields::field5,
            comms::option::FixedLength<6>,
            comms::option::ValidNumValueRange<static_cast<std::int64_t>(0xffff800000000000), 0x7fffffffffff>,
            comms::option::def::HasName
        >;

    public:
        using ValueType = typename Base::ValueType;
        using SpecialNameInfo = std::pair<ValueType, const char*>;
        using SpecialNamesMapInfo = std::pair<const SpecialNameInfo*, std::size_t>;

        static constexpr bool hasSpecials()
        {
            return true;
        }

        static const char* name()
        {
            return "field5";
        }  

        static SpecialNamesMapInfo specialNamesMap()
        {
            static const SpecialNameInfo Map[] = {
                std::make_pair(ValueType(0xffffff), "S1"),
                std::make_pair(ValueType(0xffffffffffff), "S2"),
            };
            static const std::size_t MapSize = std::extent<decltype(Map)>::value;

            return std::make_pair(&Map[0], MapSize);
        }                       
    };

    /// @brief Unsigned integer serialised using 8 bytes
    class field6 : public
        comms::field::IntValue<
            demo::FieldBase,
            std::uint64_t,
            typename TOpt::message::IntValuesFields::field6,
            comms::option::def::HasName
        >
    {
    public:
        static constexpr bool hasSpecials()
        {
            return false;
        }

        static const char* name()
        {
            return "field6";
        }                 
    };


    /// @brief All the fields bundled in std::tuple.
    using All = std::tuple<
        field1,
        field2,
        field3,
        field4,
        field5,
        field6
    >;
};

/// @brief Accumulates various integer values.
/// @details Inherits from
///     @b comms::MessageBase
///     while providing @b TMsgBase as common interface class as well as
///     various implementation options. @n
///     See @ref IntValuesFields for definition of the fields this message contains.
/// @tparam TMsgBase Common interface class for all the messages.
/// @tparam TOpt Extra options
template <typename TMsgBase, typename TOpt = demo::DefaultOptions>
class IntValues : public
    comms::MessageBase<
        TMsgBase,
        typename TOpt::message::IntValues,
        comms::option::StaticNumIdImpl<MsgId_IntValues>,
        comms::option::FieldsImpl<typename IntValuesFields<TOpt>::All>,
        comms::option::MsgType<IntValues<TMsgBase, TOpt> >,
        comms::option::HasName
    >
{
    // Required for compilation with gcc earlier than v5.0,
    // later versions don't require this type definition.
    using Base =
        comms::MessageBase<
            TMsgBase,
            typename TOpt::message::IntValues,
            comms::option::StaticNumIdImpl<MsgId_IntValues>,
            comms::option::FieldsImpl<typename IntValuesFields<TOpt>::All>,
            comms::option::MsgType<IntValues<TMsgBase, TOpt> >,
            comms::option::HasName
        >;

    static const bool AreFieldsVersionDependent = Base::areFieldsVersionDependent();
    static_assert(!AreFieldsVersionDependent, "Fields mustn't be version dependent");
public:

    /// @brief Allow access to internal fields.
    /// @details See definition of @b COMMS_MSG_FIELDS_NAMES macro
    ///     related to @b comms::MessageBase class from COMMS library
    ///     for details.
    ///
    COMMS_MSG_FIELDS_NAMES(field1, field2, field3, field4, field5, field6);

    // Check serialisation lengths
    // For some reason VS2015 compiler fails when calls to doMinLength() and
    // doMaxLength() are performed inside static_assert.
    static const std::size_t MsgMinLen = Base::doMinLength();
    static const std::size_t MsgMaxLen = Base::doMaxLength();
    static_assert(MsgMinLen == 21, "Unexpected min serialisation length");
    static_assert(MsgMaxLen == 24, "Unexpected max serialisation length");

    /// @brief Default constructor
    IntValues() = default;

    /// @brief Copy constructor
    IntValues(const IntValues&) = default;

    /// @brief Move constructor
    IntValues(IntValues&& other) = default;

    /// @brief Destructor
    ~IntValues() noexcept = default;

    /// @brief Copy assignment
    IntValues& operator=(const IntValues&) = default;

    /// @brief Move assignment
    IntValues& operator=(IntValues&&) = default;

    /// @brief Name of the message.
    static const char* doName()
    {
        return "IntValues";
    }
};

}  // namespace message

}  // namespace demo


