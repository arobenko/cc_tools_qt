//
// Copyright 2014 - 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#pragma once

#include <chrono>
#include <vector>
#include <string>
#include <memory>

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtCore/QVariant>
#include <QtCore/QVariantMap>
CC_ENABLE_WARNINGS()

#include "Api.h"

namespace comms_champion
{

struct DataInfo
{
    typedef std::chrono::high_resolution_clock TimestampClock;
    typedef std::chrono::time_point<TimestampClock> Timestamp;
    typedef std::vector<std::uint8_t> DataSeq;
    typedef QVariantMap PropertiesMap;

    Timestamp m_timestamp;
    DataSeq m_data;
    PropertiesMap m_extraProperties;
};

using DataInfoPtr = std::shared_ptr<DataInfo>;

CC_API DataInfoPtr makeDataInfo();

}  // namespace comms_champion

Q_DECLARE_METATYPE(comms_champion::DataInfoPtr);


