//
// Copyright 2015 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "cc_tools_qt/ToolsPlugin.h"

namespace cc_tools_qt
{

namespace
{

template <typename TFunc>
auto invokeCreationFunc(TFunc&& func) -> decltype(func())
{
    typedef decltype(func()) RetType;

    if (!func) {
        return RetType();
    }

    return func();
}

}  // namespace

ToolsPlugin::ToolsPlugin(Type type) :
    m_type(type)
{
    assert(type < Type_NumOfValues);
}

ToolsPlugin::~ToolsPlugin() noexcept = default;

ToolsPlugin::Type ToolsPlugin::type() const
{
    return m_type;
}

void ToolsPlugin::getCurrentConfig(QVariantMap& config)
{
    getCurrentConfigImpl(config);
}

QVariantMap ToolsPlugin::getCurrentConfig()
{
    QVariantMap config;
    getCurrentConfig(config);
    return config;
}

void ToolsPlugin::reconfigure(const QVariantMap& config)
{
    reconfigureImpl(config);
}

ToolsSocketPtr ToolsPlugin::createSocket()
{
    if (m_type != Type_Socket) {
        return ToolsSocketPtr();
    }

    auto socketPtr = createSocketImpl();
    assert(socketPtr); // Must override
    if (socketPtr) {
        socketPtr->setDebugOutputLevel(m_debugOutputLevel);
    }
    return socketPtr;
}

ToolsFilterPtr ToolsPlugin::createFilter()
{
    if (m_type != Type_Filter) {
        return ToolsFilterPtr();
    }

    auto filterPtr = createFilterImpl();
    assert(filterPtr);
    if (filterPtr) {
        filterPtr->setDebugOutputLevel(m_debugOutputLevel);
    }
    return filterPtr;
}

ToolsProtocolPtr ToolsPlugin::createProtocol()
{
    if (m_type != Type_Protocol) {
        return ToolsProtocolPtr();
    }

    auto protocolPtr = createProtocolImpl();
    assert(protocolPtr);
    if (protocolPtr) {
        protocolPtr->setDebugOutputLevel(m_debugOutputLevel);
    }

    return protocolPtr;
}

QWidget* ToolsPlugin::createConfigurationWidget()
{
    return createConfigurationWidgetImpl();
}

ToolsPlugin::ListOfGuiActions ToolsPlugin::createGuiActions()
{
    return createGuiActionsImpl();
}

void ToolsPlugin::applyInterPluginConfig(const QVariantMap& props)
{
    applyInterPluginConfigImpl(props);
}

void ToolsPlugin::setInterPluginConfigReportCallback(InterPluginConfigReportCallback&& func)
{
    m_interPluginConfigReportCallback = std::move(func);
}    

void ToolsPlugin::setDebugOutputLevel(unsigned level)
{
    m_debugOutputLevel = level;
}

void ToolsPlugin::getCurrentConfigImpl([[maybe_unused]] QVariantMap& config)
{
}

void ToolsPlugin::reconfigureImpl([[maybe_unused]] const QVariantMap& config)
{
}

void ToolsPlugin::applyInterPluginConfigImpl([[maybe_unused]] const QVariantMap& props)
{
}

ToolsSocketPtr ToolsPlugin::createSocketImpl()
{
    return ToolsSocketPtr();
}

ToolsFilterPtr ToolsPlugin::createFilterImpl()
{
    return ToolsFilterPtr();
}

ToolsProtocolPtr ToolsPlugin::createProtocolImpl()
{
    return ToolsProtocolPtr();
}

QWidget* ToolsPlugin::createConfigurationWidgetImpl()
{
    return nullptr;
}

ToolsPlugin::ListOfGuiActions ToolsPlugin::createGuiActionsImpl()
{
    return ListOfGuiActions();
}

void ToolsPlugin::reportInterPluginConfig(const QVariantMap& props)
{
    if (m_interPluginConfigReportCallback) {
        m_interPluginConfigReportCallback(props);
    }
}

}  // namespace cc_tools_qt


