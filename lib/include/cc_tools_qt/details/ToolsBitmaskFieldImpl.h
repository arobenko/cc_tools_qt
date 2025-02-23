//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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


#pragma once

#include "cc_tools_qt/details/ToolsNumericFieldImpl.h"
#include "cc_tools_qt/field/ToolsBitmaskField.h"

#include "comms/field/BitmaskValue.h"

#include <cassert>
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>

namespace cc_tools_qt
{

namespace details
{

template <typename TField>
class ToolsBitmaskFieldImpl : public ToolsNumericFieldImpl<cc_tools_qt::field::ToolsBitmaskField, TField>
{
    using Base = ToolsNumericFieldImpl<cc_tools_qt::field::ToolsBitmaskField, TField>;
    using Field = TField;
    static_assert(comms::field::isBitmaskValue<Field>(), "Must be of BitmaskValueField type");

    using ValueType = typename Field::ValueType;
    using MaskType = typename Base::UnderlyingType;
    static_assert(sizeof(ValueType) <= sizeof(MaskType), "This class cannot handle provided field.");

public:
    using Ptr = typename Base::Ptr;
    using ActPtr = typename Base::ActPtr;

    explicit ToolsBitmaskFieldImpl(Field& fieldRef)
      : Base(fieldRef)
    {
    }

    ToolsBitmaskFieldImpl(const ToolsBitmaskFieldImpl&) = default;
    ToolsBitmaskFieldImpl(ToolsBitmaskFieldImpl&&) = default;
    virtual ~ToolsBitmaskFieldImpl() noexcept = default;

    ToolsBitmaskFieldImpl& operator=(const ToolsBitmaskFieldImpl&) = delete;

protected:
    virtual bool bitValueImpl(unsigned idx) const override
    {
        return Base::field().getBitValue(idx);
    }

    virtual void setBitValueImpl(unsigned idx, bool value) override
    {
        using Tag = 
            std::conditional_t<
                Field::hasFixedValue(),
                NoFeatureTag,
                HasFeatureTag
            >;
            
        setBitValueInternal(idx, value, Tag());
    }

    virtual unsigned bitIdxLimitImpl() const override
    {
        return std::numeric_limits<ValueType>::digits;
    }

    virtual Ptr cloneImpl() override
    {
        return ActPtr(new ToolsBitmaskFieldImpl<TField>(Base::field()));
    }

    virtual const QStringList& bitsImpl() const override
    {
        static const QStringList Bits = createBitsList();
        return Bits;
    }

private:
    struct HasFeatureTag {};
    struct NoFeatureTag {};

    static QStringList createBitsList()
    {
        QStringList result;
        for (auto idx = 0U; idx < Field::BitIdx_numOfValues; ++idx) {
            auto* name = Field::bitName(idx);
            if (name == nullptr) {
                result.append(QString());
                continue;
            }

            result.append(name);
        }
        return result;
    }

    void setBitValueInternal(unsigned idx, bool value, HasFeatureTag)
    {
        Base::field().setBitValue(idx, value);
    }    

    void setBitValueInternal([[maybe_unused]] unsigned idx, [[maybe_unused]] bool value, NoFeatureTag)
    {
        [[maybe_unused]] static constexpr bool Must_not_be_called = false;
        assert(Must_not_be_called);
    }        
};
template <typename TField>
auto makeBitmaskField(TField& field)
{
    return std::make_unique<ToolsBitmaskFieldImpl<TField>>(field);
}

}  // namespace details

}  // namespace cc_tools_qt
