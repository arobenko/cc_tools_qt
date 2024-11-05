//
// Copyright 2016 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsSocket.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace cc_tools_qt
{


namespace 
{

const std::string& debugPrefix()
{
    static const std::string Str("(socket)");
    return Str;
}    

std::string dataToStr(const ToolsDataInfo::DataSeq& data)
{
    std::stringstream stream;
    stream << std::hex;

    for (auto byte : data) {
        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(byte) << ' ';
    }

    return stream.str();
}

} // namespace 

ToolsSocket::ToolsSocket() = default;
ToolsSocket::~ToolsSocket() noexcept = default;

bool ToolsSocket::start()
{
    m_running = startImpl();
    return m_running;
}

void ToolsSocket::stop()
{
    if (m_connected) {
        socketDisconnect();
        reportDisconnected();
    }
    m_running = false;
    stopImpl();
}

bool ToolsSocket::isRunning() const
{
    return m_running;
}

bool ToolsSocket::socketConnect()
{
    m_connected = socketConnectImpl();
    if (m_connectionStatusReportCallback) {
        m_connectionStatusReportCallback(m_connected);
    }    
    return m_connected;
}

void ToolsSocket::socketDisconnect()
{
    socketDisconnectImpl();
    m_connected = false;
    if (m_connectionStatusReportCallback) {
        m_connectionStatusReportCallback(false);
    }      
}

bool ToolsSocket::isSocketConnected() const
{
    return m_connected;
}

void ToolsSocket::sendData(ToolsDataInfoPtr dataPtr)
{
    if (!isSocketConnected()) {
        return;
    }

    if (dataPtr->m_timestamp == ToolsDataInfo::Timestamp()) {
        dataPtr->m_timestamp = ToolsDataInfo::TimestampClock::now();
    }

    if (1U < m_debugLevel) {
        auto sinceEpoch = dataPtr->m_timestamp.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        std::cout << '[' << milliseconds << "] " << debugPrefix() << " --> " << dataPtr->m_data.size() << " bytes"; 
        if (2U < m_debugLevel) {
            std::cout << " | " << dataToStr(dataPtr->m_data);
        }
        std::cout << std::endl;
    }

    sendDataImpl(std::move(dataPtr));
}

unsigned ToolsSocket::connectionProperties() const
{
    return connectionPropertiesImpl();
}

void ToolsSocket::applyInterPluginConfig(const QVariantMap& props)
{
    applyInterPluginConfigImpl(props);
}

void ToolsSocket::setDebugOutputLevel(unsigned level)
{
    m_debugLevel = level;
}

bool ToolsSocket::startImpl()
{
    return true;
}

void ToolsSocket::stopImpl()
{
}

bool ToolsSocket::socketConnectImpl()
{
    return true;
}

void ToolsSocket::socketDisconnectImpl()
{
}

unsigned ToolsSocket::connectionPropertiesImpl() const
{
    return 0U;
}

void ToolsSocket::applyInterPluginConfigImpl([[maybe_unused]] const QVariantMap& props)
{
}

void ToolsSocket::reportDataReceived(ToolsDataInfoPtr dataPtr)
{
    if ((!m_running) || (!m_dataReceivedCallback)) {
        return;
    }

    if (dataPtr->m_timestamp == ToolsDataInfo::Timestamp()) {
        dataPtr->m_timestamp = ToolsDataInfo::TimestampClock::now();
    }

    if (1U <= m_debugLevel) {
        auto sinceEpoch = dataPtr->m_timestamp.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
        std::cout << '[' << milliseconds << "] " << debugPrefix() << " <-- " << dataPtr->m_data.size() << " bytes"; 
        if (2U <= m_debugLevel) {
            std::cout << " | " << dataToStr(dataPtr->m_data);
        }
        std::cout << std::endl;
    }

    m_dataReceivedCallback(std::move(dataPtr));
}

void ToolsSocket::reportError(const QString& msg)
{
    if (m_running && m_errorReportCallback) {
        m_errorReportCallback(msg);
    }
}

void ToolsSocket::reportDisconnected()
{
    m_connected = false;
    if (m_running && m_connectionStatusReportCallback) {
        m_connectionStatusReportCallback(false);
    }
}

void ToolsSocket::reportInterPluginConfig(const QVariantMap& props)
{
    if (m_interPluginConfigReportCallback) {
        m_interPluginConfigReportCallback(props);
    }
}

unsigned long long ToolsSocket::currTimestamp()
{
    auto timestamp = std::chrono::high_resolution_clock::now();
    auto sinceEpoch = timestamp.time_since_epoch();
    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch).count();
    return milliseconds;
}

unsigned ToolsSocket::getDebugOutputLevel() const
{
    return m_debugLevel;
}

}  // namespace cc_tools_qt
