//
// Copyright 2024 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "comms/options.h"

#include <cstdint>
#include <iterator>
#include <vector>

namespace cc_tools_qt
{

/// @brief Interface class that needs to be used for the messages when implementing protocol plugin 
/// @details Extends comms::Message as well as provided interface class, while
///     passing required options to the latter.
/// @tparam TMessageBase Name of the interface class defined for custom
///     protocol. Expected to inherit (or typedef) from @b comms::Message
///     and have the following definition:
///     @code
///     template <typename... TOptions>
///     class MyInterfaceClass : public comms::Message<...> {...};
///     @endcode
/// @headerfile cc_tools_qt/ToolsMessageInterface.h
template <template<typename...> class TMessageBase>
using ToolsMessageInterface = 
    TMessageBase<
//        comms::option::IdInfoInterface,
//        comms::option::ReadIterator<const std::uint8_t*>,
//        comms::option::WriteIterator<std::back_insert_iterator<std::vector<std::uint8_t>>>,
//        comms::option::Handler<MessageHandler>,
//        comms::option::ValidCheckInterface,
//        comms::option::LengthInfoInterface,
//        comms::option::RefreshInterface,
//        comms::option::NameInterface,        
    >;

} // namespace cc_tools_qt
