//
// Copyright 2014 - 2025 (C). Alex Robenko. All rights reserved.
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

#include <QtWidgets/QDialog>

#include "ui_MessageUpdateDialog.h"

#include "cc_tools_qt/ToolsProtocol.h"
#include "MessageDisplayWidget.h"

namespace cc_tools_qt
{

class MessageUpdateDialog : public QDialog
{
    Q_OBJECT
    using Base = QDialog;
public:
    MessageUpdateDialog(
        ToolsMessagePtr& msg,
        ToolsProtocolPtr protocol,
        QWidget* parentObj = nullptr);

private slots:
    void msgUpdated();
    void newItemSelected();
    void displayMessagePostponed(ToolsMessagePtr msg, bool force);
    void refreshDisplayedList(const QString& searchText);
    void refreshDelayInfo(int checkboxValue);
    void delayUpdated(int value);
    void refreshRepeatInfo(int checkboxValue);
    void repeatDurationUpdated(int value);
    void repeatCountUpdated(int value);
    void indefinitelyUpdated(int checkboxValue);
    void accept();
    void reset();

private:
    ToolsMessagePtr getMsgFromItem(QListWidgetItem* item);
    void refreshButtons();

    ToolsMessagePtr& m_msg;
    ToolsProtocolPtr m_protocol;
    ToolsMessagesList m_allMsgs;
    MessageDisplayWidget* m_msgDisplayWidget = nullptr;
    Ui::MessageUpdateDialog m_ui;
    int m_prevDelay = DisabledDelayValue + 1;
    int m_prevRepeatDuration = DisabledRepeatDuration + 1;
    int m_prevRepeatCount = EnabledMinRepeatCount;
    Qt::CheckState m_sendIndefinitelyState = DisabledSendIndefinitelyState;
    int m_origScrollPos = 0;

    static const int DisabledDelayValue = 0;
    static const int DisabledRepeatDuration = 0;
    static const int DisabledRepeatCount = 1;
    static const int EnabledMinRepeatCount = 2;
    static const Qt::CheckState DisabledSendIndefinitelyState = Qt::Unchecked;
};

}  // namespace cc_tools_qt

