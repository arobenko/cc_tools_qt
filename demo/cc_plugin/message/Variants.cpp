//
// Copyright 2017 - 2018 (C). Alex Robenko. All rights reserved.
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

#include "Variants.h"

#include "cc_tools_qt/property/field.h"

#include <cassert>

namespace cc = cc_tools_qt;

namespace demo
{

namespace cc_plugin
{

namespace message
{

namespace
{

using ProtMsg = demo::cc_plugin::message::Variants::ProtMsg;

QVariantMap createProps_field1()
{
    using Field = ProtMsg::Field_field1;
    auto props = cc::property::field::ForField<Field>();
    return props.asMap();
}

QVariantMap createProps_field2()
{
    using Field = ProtMsg::Field_field2;
    auto props = cc::property::field::ForField<Field>();
    return props.asMap();
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createProps_field1());
    props.append(createProps_field2());

    assert(props.size() == ProtMsg::FieldIdx_numOfValues);
    return props;
}

}  // namespace

Variants::Variants() = default;
Variants::~Variants() noexcept = default;

Variants& Variants::operator=(const Variants&) = default;
Variants& Variants::operator=(Variants&&) = default;

const QVariantList& Variants::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace demo

