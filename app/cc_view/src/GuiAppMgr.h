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

#include <memory>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

#include "cc_tools_qt/ToolsMessage.h"
#include "cc_tools_qt/ToolsPluginMgr.h"
#include "cc_tools_qt/ToolsMsgSendMgr.h"
#include "cc_tools_qt/ToolsProtocol.h"

#include "ActionWrap.h"
#include "MsgMgrG.h"

namespace cc_tools_qt
{

class GuiAppMgr : public QObject
{
    Q_OBJECT
    typedef QObject Base;
public:

    enum class RecvState {
        Idle,
        Running,
        NumOfStates
    };

    enum class SendState {
        Idle,
        SendingSingle,
        SendingAll,
        NumOfStates
    };

    enum RecvListMode : unsigned {
        RecvListMode_ShowReceived = 1U << 0,
        RecvListMode_ShowSent = 1U << 1,
        RecvListMode_ShowGarbage = 1U << 2,
        RecvListMode_ApplyFilter = 1U << 3
    };

    using MsgType = ToolsMsgMgr::MsgType;
    using ActionPtr = std::shared_ptr<QAction>;
    using ListOfPluginInfos = ToolsPluginMgr::ListOfPluginInfos;
    using FilteredMessages = std::vector<QString>;

    enum class ActivityState
    {
        Clear,
        Inactive,
        Active,
    };

    static GuiAppMgr* instance();
    static GuiAppMgr& instanceRef();

    ~GuiAppMgr() noexcept;

    bool startClean();
    bool startFromConfig(const QString& configName);
    bool startFromFile(const QString& filename);

    RecvState recvState() const;
    bool recvMsgListSelectOnAddEnabled();
    bool recvListEmpty() const;
    void recvLoadMsgsFromFile(const QString& filename);
    void recvSaveMsgsToFile(const QString& filename);
    bool recvListShowsReceived() const;
    bool recvListShowsSent() const;
    bool recvListShowsGarbage() const;
    bool recvListApplyFilter() const;
    unsigned recvListModeMask() const;

    SendState sendState() const;
    void sendAddNewMessage(ToolsMessagePtr msg);
    void sendUpdateMessage(ToolsMessagePtr msg);
    bool sendListEmpty() const;
    void sendLoadMsgsFromFile(bool clear, const QString& filename);
    void sendSaveMsgsToFile(const QString& filename);
    void sendUpdateList(const ToolsMessagesList& msgs);

    void deleteMessages(ToolsMessagesList&& msgs);
    void sendMessages(ToolsMessagesList&& msgs);

    static ActivityState getActivityState();
    bool applyNewPlugins(const ListOfPluginInfos& plugins);
    void msgCommentUpdated(ToolsMessagePtr msg);

    const FilteredMessages& getFilteredMessages() const;
    void setFilteredMessages(FilteredMessages&& filteredMessages);

    static QString messageDesc(const ToolsMessage& msg);

    void setDebugOutputLevel(unsigned level);

public slots:
    void pluginsEditClicked();

    void recvStartClicked();
    void recvStopClicked();
    void recvLoadClicked();
    void recvSaveClicked();
    void recvCommentClicked();
    void recvDupClicked();
    void recvDeleteClicked();
    void recvClearClicked();
    void recvEditFilterClicked();
    void recvShowRecvToggled(bool checked);
    void recvShowSentToggled(bool checked);
    void recvShowGarbageToggled(bool checked);
    void recvApplyFilterToggled(bool checked);

    void sendStartClicked();
    void sendStartAllClicked();
    void sendStopClicked();
    void sendLoadClicked();
    void sendSaveClicked();
    void sendAddClicked();
    void sendAddRawClicked();
    void sendEditClicked();
    void sendCommentClicked();
    void sendDupClicked();
    void sendDeleteClicked();
    void sendClearClicked();
    void sendTopClicked();
    void sendUpClicked();
    void sendDownClicked();
    void sendBottomClicked();

    void recvMsgClicked(ToolsMessagePtr msg, int idx);

    void sendMsgClicked(ToolsMessagePtr msg, int idx);
    void sendMsgDoubleClicked(ToolsMessagePtr msg, int idx);
    void sendSelectedMsgMoved(int idx);

    void addMainToolbarAction(ActionPtr action);

    void connectSocketClicked();
    void disconnectSocketClicked();

signals:
    void sigAddRecvMsg(ToolsMessagePtr msg);
    void sigAddSendMsg(ToolsMessagePtr msg);
    void sigSendMsgUpdated(ToolsMessagePtr msg);
    void sigSetRecvState(int state);
    void sigSetSendState(int state);
    void sigDisplayMsgDetailsWidget(QWidget* widget);
    void sigRecvMsgListSelectOnAddEnabled(bool enabled);
    void sigRecvMsgListClearSelection();
    void sigSendMsgListClearSelection();
    void sigDisplayMsg(ToolsMessagePtr msg);
    void sigClearDisplayedMsg();
    void sigRecvMsgSelected(int index);
    void sigSendMsgSelected(int index);
    void sigRecvDeleteSelectedMsg();
    void sigSendDeleteSelectedMsg();
    void sigRecvClear(bool reportDeleted);
    void sigSendClear();
    void sigRecvListCountReport(unsigned count);
    void sigSendListCountReport(unsigned count);
    void sigSendMoveSelectedTop();
    void sigSendMoveSelectedUp();
    void sigSendMoveSelectedDown();
    void sigSendMoveSelectedBottom();
    void sigRecvListTitleNeedsUpdate();
    void sigNewSendMsgDialog(ToolsProtocolPtr protocol);
    void sigSendRawMsgDialog(ToolsProtocolPtr protocol);
    void sigUpdateSendMsgDialog(ToolsMessagePtr msg, ToolsProtocolPtr protocol);
    void sigLoadRecvMsgsDialog();
    void sigSaveRecvMsgsDialog();
    void sigLoadSendMsgsDialog(bool askForClear);
    void sigSaveSendMsgsDialog();
    void sigPluginsEditDialog();
    void sigActivityStateChanged(int value);
    void sigErrorReported(const QString& msg);
    void sigAddMainToolbarAction(ActionPtr action);
    void sigClearAllMainToolbarActions();
    void sigRecvSaveMsgs(const QString& filename);
    void sigSendLoadMsgs(bool clear, const QString& filename, ToolsProtocolPtr protocol);
    void sigSendSaveMsgs(const QString& filename);
    void sigSocketConnected(bool connected);
    void sigSocketConnectEnabled(bool enabled);
    void sigMsgCommentDialog(ToolsMessagePtr msg);
    void sigMsgCommentUpdated(ToolsMessagePtr msg);
    void sigRecvFilterDialog(ToolsProtocolPtr protocol);

private:
    enum class SelectionType
    {
        None,
        Recv,
        Send
    };

    GuiAppMgr(QObject* parentObj = nullptr);
    void emitRecvStateUpdate();
    void emitSendStateUpdate();

private slots:
    void msgAdded(ToolsMessagePtr msg);
    void errorReported(const QString& msg);
    void pendingDisplayTimeout();

private /*data*/:

    void msgClicked(ToolsMessagePtr msg, SelectionType selType);
    void displayMessage(ToolsMessagePtr msg);
    void clearDisplayedMessage();
    void refreshRecvList();
    void addMsgToRecvList(ToolsMessagePtr msg);
    void clearRecvList(bool reportDeleted);
    bool canAddToRecvList(const ToolsMessage& msg, MsgType type) const;
    void decRecvListCount();
    void decSendListCount();
    void emitRecvNotSelected();
    void emitSendNotSelected();
    void updateRecvListMode(RecvListMode mode, bool checked);
    void refreshRecvState();

    RecvState m_recvState = RecvState::Running;
    bool m_recvListSelectOnAdd = true;
    unsigned m_recvListCount = 0;
    unsigned m_recvListMode =
        RecvListMode_ShowReceived |
        RecvListMode_ShowSent |
        RecvListMode_ShowGarbage |
        RecvListMode_ApplyFilter;

    SendState m_sendState = SendState::Idle;
    unsigned m_sendListCount = 0;

    SelectionType m_selType = SelectionType::None;
    ToolsMessagePtr m_clickedMsg;

    QTimer m_pendingDisplayTimer;
    ToolsMessagePtr m_pendingDisplayMsg;
    bool m_pendingDisplayWaitInProgress = false;

    ToolsMsgSendMgr m_sendMgr;

    FilteredMessages m_filteredMessages;

    unsigned m_debugOutputLevel = 0U;
};

}  // namespace cc_tools_qt

Q_DECLARE_METATYPE(cc_tools_qt::GuiAppMgr::ActionPtr);
