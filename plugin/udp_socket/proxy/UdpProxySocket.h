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

#pragma once

#include "cc_tools_qt/ToolsSocket.h"

#include <QtNetwork/QUdpSocket>

#include <memory>

#ifdef CC_TOOLS_QT_DEFAULT_NETWORK_PORT
#define UDP_PROXY_DEFAULT_PORT CC_TOOLS_QT_DEFAULT_NETWORK_PORT    
#else
#define UDP_PROXY_DEFAULT_PORT 20000
#endif

namespace cc_tools_qt
{

namespace plugin
{

class UdpProxySocket : public cc_tools_qt::ToolsSocket
{
    Q_OBJECT
    using Base = cc_tools_qt::ToolsSocket;

public:
    typedef unsigned short PortType;

    UdpProxySocket();
    ~UdpProxySocket() noexcept;

    void setHost(const QString& value)
    {
        m_host = value;
    }

    const QString& getHost() const
    {
        return m_host;
    }

    void setPort(PortType value)
    {
        m_port = value;
    }

    PortType getPort() const
    {
        return m_port;
    }

    void setLocalPort(PortType value)
    {
        m_localPort = value;
    }

    PortType getLocalPort() const
    {
        return m_localPort;
    }

signals:
    void sigConfigChanged();    

protected:
    virtual bool socketConnectImpl() override;
    virtual void socketDisconnectImpl() override;
    virtual void sendDataImpl(ToolsDataInfoPtr dataPtr) override;
    virtual void applyInterPluginConfigImpl(const QVariantMap& props) override;     

private slots:
    void listenSocketDisconnected();
    void readFromListenSocket();
    void listenSocketErrorOccurred(QAbstractSocket::SocketError err);
    void remoteSocketDisconnected();
    void readFromRemoteSocket();
    void remoteSocketErrorOccurred(QAbstractSocket::SocketError err);

private:
    using SocketPtr = std::unique_ptr<QUdpSocket>;

    void readData(QUdpSocket& socket);
    bool createListenSocket();
    void createRemoteSocketIfNeeded();

    static const PortType DefaultPort = UDP_PROXY_DEFAULT_PORT;

    QString m_host;
    PortType m_port = DefaultPort;
    PortType m_localPort = DefaultPort + 1;
    SocketPtr m_listenSocket;
    SocketPtr m_remoteSocket;
    bool m_running = false;
};

} // namespace plugin

} // namespace cc_tools_qt
