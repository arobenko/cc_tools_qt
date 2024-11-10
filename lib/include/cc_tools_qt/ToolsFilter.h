//
// Copyright 2014 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsApi.h"
#include "cc_tools_qt/ToolsDataInfo.h"

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>

#include <cstdint>
#include <cstddef>
#include <vector>
#include <functional>

namespace cc_tools_qt
{

/// @brief Main polymorphic interface class for filters.
/// @details The filter can be used to implement encryption / decryption of
///     the data, and/or additional transport layer.
/// @headerfile cc_tools_qt/ToolsFilter.h
class CC_TOOLS_API ToolsFilter : public QObject
{
    Q_OBJECT
public:
    /// @brief Constructor
    ToolsFilter();

    /// @brief Destructor
    virtual ~ToolsFilter() noexcept;

    /// @brief Start filter's operation
    /// @details Prior to being started, the filter is not allowed to generate
    ///     any events. The function invokes virtual startImpl() which can
    ///     be overriden by the derived class.
    /// @return true in case the socket's operation has been successfully started,
    ///     false otherwise.
    bool start();

    /// @brief Stop filter's operation.
    /// @details After being stopped, the filter is not allowed to generate any
    ///     events. The function invokes virtual stopImpl(), which can be
    ///     overriden by the derived class
    void stop();

    /// @brief Process received data
    /// @details Process the data, received by I/O socket or other filter
    ///     down the chain.
    /// @param[in] dataPtr Incoming data from I/O socket or other filter
    ///     down the chain
    /// @return Data to forward to the protocol or to other filter up the
    ///     chain
    QList<ToolsDataInfoPtr> recvData(ToolsDataInfoPtr dataPtr);

    /// @brief Process outgoing data
    /// @details Process the data, generated by the protocol or other filter
    ///     up the chain.
    /// @param[in] dataPtr Outgoing data generated by the protocol or
    ///     other filter up the chain.
    /// @return Data to forward to the I/O socket or other filter down the
    ///     chain
    QList<ToolsDataInfoPtr> sendData(ToolsDataInfoPtr dataPtr);

    /// @brief Make the filter aware about socket connection status
    /// @param[in] connected Socket connection status.
    void socketConnectionReport(bool connected);

    /// @brief Apply inter-plugin configuration.
    /// @details Allows one plugin to influence the configuration of another.
    ///     This function will be called for all currently chosen plugins to override
    ///     current configuration. Invokes polymorphic @ref applyInterPluginConfigImpl().
    /// @param[in] props Properties map.
    void applyInterPluginConfig(const QVariantMap& props);     

    /// @brief Set debug output level
    /// @param[in] level Debug level. If @b 0, debug output is disabled
    void setDebugOutputLevel(unsigned level = 0U);    

signals:
    void sigDataToSendReport(ToolsDataInfoPtr data);
    void sigErrorReport(const QString& msg);
    void sigInterPluginConfigReport(const QVariantMap& props);    

protected:
    /// @brief Polymorphic start functionality implementation.
    /// @details Invoked by start() and default implementation does nothing.
    ///     It can be overrided by the derived class.
    /// @return true
    virtual bool startImpl();

    /// @brief Polymorphic stop functionality implementation.
    /// @details Invoked by stop() and default implementation does nothing.
    ///     It can be overridden by the derived class.
    virtual void stopImpl();

    /// @brief Polymorphic processing of incoming data.
    /// @details Invoked by recvData() and expected be overridden in the derived
    ///     class. Default implementation is pass-through of data.
    virtual QList<ToolsDataInfoPtr> recvDataImpl(ToolsDataInfoPtr dataPtr);

    /// @brief Polymorphic processing of outgoing data.
    /// @details Invoked by sendData() and expected be overridden in the derived
    ///     class. Default implementation is pass-through of data.
    virtual QList<ToolsDataInfoPtr> sendDataImpl(ToolsDataInfoPtr dataPtr);

    /// @brief Polymorphic processing of the socket connection report
    /// @param[in] connected Socket connection status
    virtual void socketConnectionReportImpl(bool connected);

    /// @brief Polymorphic inter-plugin configuration application.
    /// @details Invoked by the applyInterPluginConfig().
    /// @param[in] props Properties map.
    virtual void applyInterPluginConfigImpl(const QVariantMap& props);      

    /// @brief Get plugin name for the debug output
    virtual const char* debugNameImpl() const;

    /// @brief Report new data to send generated by the filter itself.
    /// @details This function needs to be invoked by the derived class when
    ///     when it has new data to be sent over I/O link. This function
    ///     will invoke callback set by setDataToSendCallback().
    /// @param[in] dataPtr New data information.
    void reportDataToSend(ToolsDataInfoPtr dataPtr);

    /// @brief Report operation error.
    /// @details This function is expected to be invoked by the derived class,
    ///     when some error is detected. This function will invoke
    ///     callback set by @ref setErrorReportCallback().
    /// @param[in] msg Error message.
    void reportError(const QString& msg);

    /// @brief Report inter-plugin configuration.
    /// @details Sometimes configuration of one plugin may influence configuration of another.
    ///     Use this function to report inter-plugin configuration properties.
    ///     When invoked all other plugins are expected to get their respecitve 
    ///     @ref applyInterPluginConfig() functions invoked.
    /// @param[in] props Reported properties.
    void reportInterPluginConfig(const QVariantMap& props);    

    /// @brief Get current timestamp
    static unsigned long long currTimestamp();

    /// @brief Get current debug output level
    unsigned getDebugOutputLevel() const;

private:
    struct InnerState;
    std::unique_ptr<InnerState> m_state;
};

/// @brief Pointer to @ref ToolsFilter object.
using ToolsFilterPtr = std::shared_ptr<ToolsFilter>;

}  // namespace cc_tools_qt

