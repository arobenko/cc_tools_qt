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

#include "DemoProtocol.h"

#include "comms/comms.h"

namespace demo
{

namespace cc_plugin
{

DemoProtocol::~DemoProtocol() noexcept = default;

const QString& DemoProtocol::nameImpl() const
{
    static const QString& Str("Demo");
    return Str;
}

}  // namespace cc_plugin

}  // namespace demo

