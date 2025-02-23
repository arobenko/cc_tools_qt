//
// Copyright 2015 - 2025 (C). Alex Robenko. All rights reserved.
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

#include "TcpProxySocketConfigWidget.h"

#include <limits>

namespace cc_tools_qt
{

namespace plugin
{

TcpProxySocketConfigWidget::TcpProxySocketConfigWidget(
    TcpProxySocket& socket,
    QWidget* parentObj)
  : Base(parentObj),
    m_socket(socket)
{
    m_ui.setupUi(this);

    m_ui.m_localPortSpinBox->setRange(
        1,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    m_ui.m_remotePortSpinBox->setRange(
        1,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    refresh();        

    connect(
        &socket, &TcpProxySocket::sigConfigChanged,
        this, &TcpProxySocketConfigWidget::refresh);  

    connect(
        m_ui.m_localPortSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &TcpProxySocketConfigWidget::localPortValueChanged);

    connect(
        m_ui.m_remoteHostLineEdit, &QLineEdit::textChanged,
        this, &TcpProxySocketConfigWidget::remoteHostValueChanged);

    connect(
        m_ui.m_remotePortSpinBox, qOverload<int>(&QSpinBox::valueChanged),
        this, &TcpProxySocketConfigWidget::remotePortValueChanged);
}

TcpProxySocketConfigWidget::~TcpProxySocketConfigWidget() noexcept = default;

void TcpProxySocketConfigWidget::refresh()
{
    m_ui.m_localPortSpinBox->setValue(
        static_cast<int>(m_socket.getPort()));

    m_ui.m_remoteHostLineEdit->setText(m_socket.getRemoteHost());

    m_ui.m_remotePortSpinBox->setValue(
        static_cast<int>(m_socket.getRemotePort()));    
}

void TcpProxySocketConfigWidget::localPortValueChanged(int value)
{
    m_socket.setPort(static_cast<PortType>(value));
}

void TcpProxySocketConfigWidget::remoteHostValueChanged(const QString& value)
{
    m_socket.setRemoteHost(value);
}

void TcpProxySocketConfigWidget::remotePortValueChanged(int value)
{
    m_socket.setRemotePort(static_cast<PortType>(value));
}

}  // namespace plugin

}  // namespace cc_tools_qt


