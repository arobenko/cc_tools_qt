//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/cc_tools_qt.h"
#include "DemoStack.h"
#include "DemoTransportMessage.h"

namespace demo
{

namespace cc_plugin
{

class DemoProtocol : public
    cc_tools_qt::ProtocolBase<
        cc_plugin::DemoStack,
        DemoTransportMessage
    >
{
public:
    DemoProtocol() = default;
    virtual ~DemoProtocol() noexcept;

protected:
    virtual const QString& nameImpl() const override;
};

}  // namespace cc_plugin

}  // namespace demo

