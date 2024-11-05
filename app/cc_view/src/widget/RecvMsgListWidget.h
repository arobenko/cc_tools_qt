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

#include "MsgListWidget.h"

namespace cc_tools_qt
{

class RecvMsgListWidget: public MsgListWidget
{
    using Base = MsgListWidget;
public:
    RecvMsgListWidget(QWidget* parentObj = nullptr);

protected:
    virtual void msgClickedImpl(ToolsMessagePtr msg, int idx) override;
    virtual void msgListClearedImpl(MessagesList&& msgs) override;
    virtual QString msgPrefixImpl(const ToolsMessage& msg) const override;
    virtual const QString& msgTooltipImpl() const override;
    virtual Qt::GlobalColor getItemColourImpl(MsgType type, bool valid) const override;
    virtual QString getTitleImpl() const override;
    virtual void saveMessagesImpl(const QString& filename) override;

private:
    static QString getTitlePrefix();
};

} /* namespace cc_tools_qt */
