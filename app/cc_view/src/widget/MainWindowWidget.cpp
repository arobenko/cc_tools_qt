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

#include "MainWindowWidget.h"

#include <cassert>

#include <QtWidgets/QSplitter>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QIcon>
#include <QtGui/QKeySequence>

#include "cc_tools_qt/property/message.h"
#include "LeftPaneWidget.h"
#include "RightPaneWidget.h"
#include "MessageUpdateDialog.h"
#include "MessagesFilterDialog.h"
#include "RawHexDataDialog.h"
#include "PluginConfigDialog.h"
#include "GuiAppMgr.h"
#include "MsgFileMgrG.h"
#include "icon.h"
#include "MainToolbar.h"
#include "MsgCommentDialog.h"
#include "ShortcutWrap.h"

namespace cc_tools_qt
{

MainWindowWidget::MainWindowWidget(QWidget* parentObj)
  : Base(parentObj)
{
    m_ui.setupUi(this);

    m_toolbar = new MainToolbar();
    addToolBar(m_toolbar);

    auto* splitter = new QSplitter();
    auto* leftPane = new LeftPaneWidget();
    auto* rightPane = new RightPaneWidget();
    rightPane->resize((leftPane->width() * 3) / 4, rightPane->height());
    splitter->addWidget(leftPane);
    splitter->addWidget(rightPane);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 1);
    setCentralWidget(splitter);

    new QShortcut(QKeySequence(tr("Ctrl+q")), this, SLOT(close()));

    auto* guiAppMgr = GuiAppMgr::instance();
    connect(
        guiAppMgr, SIGNAL(sigNewSendMsgDialog(ToolsProtocolPtr)),
        this, SLOT(newSendMsgDialog(ToolsProtocolPtr)));
    connect(
        guiAppMgr, SIGNAL(sigSendRawMsgDialog(ToolsProtocolPtr)),
        this, SLOT(sendRawMsgDialog(ToolsProtocolPtr)));
    connect(
        guiAppMgr, SIGNAL(sigUpdateSendMsgDialog(ToolsMessagePtr, ToolsProtocolPtr)),
        this, SLOT(updateSendMsgDialog(ToolsMessagePtr, ToolsProtocolPtr)));
    connect(
        guiAppMgr, SIGNAL(sigPluginsEditDialog()),
        this, SLOT(pluginsEditDialog()));
    connect(
        guiAppMgr, SIGNAL(sigErrorReported(const QString&)),
        this, SLOT(displayErrorMsg(const QString&)));
    connect(
        guiAppMgr, SIGNAL(sigAddMainToolbarAction(ActionPtr)),
        this, SLOT(addMainToolbarAction(ActionPtr)));
    connect(
        guiAppMgr, SIGNAL(sigClearAllMainToolbarActions()),
        this, SLOT(clearAllMainToolbarActions()));
    connect(
        guiAppMgr, SIGNAL(sigActivityStateChanged(int)),
        this, SLOT(activeStateChanged(int)));
    connect(
        guiAppMgr, SIGNAL(sigLoadRecvMsgsDialog()),
        this, SLOT(loadRecvMsgsDialog()));
    connect(
        guiAppMgr, SIGNAL(sigSaveRecvMsgsDialog()),
        this, SLOT(saveRecvMsgsDialog()));
    connect(
        guiAppMgr, SIGNAL(sigLoadSendMsgsDialog(bool)),
        this, SLOT(loadSendMsgsDialog(bool)));
    connect(
        guiAppMgr, SIGNAL(sigSaveSendMsgsDialog()),
        this, SLOT(saveSendMsgsDialog()));
    connect(
        guiAppMgr, SIGNAL(sigMsgCommentDialog(ToolsMessagePtr)),
        this, SLOT(msgCommentDialog(ToolsMessagePtr)));
    connect(
        m_ui.m_actionQuit, SIGNAL(triggered()),
        this, SLOT(close()));
    connect(
        m_ui.m_actionAbout, SIGNAL(triggered()),
        this, SLOT(aboutInfo()));
    connect(
        guiAppMgr, SIGNAL(sigRecvFilterDialog(ToolsProtocolPtr)),
        this, SLOT(recvFilterDialog(ToolsProtocolPtr)));        
}

MainWindowWidget::~MainWindowWidget() noexcept
{
    clearCustomToolbarActions();
}

void MainWindowWidget::newSendMsgDialog(ToolsProtocolPtr protocol)
{
    ToolsMessagePtr msg;
    MessageUpdateDialog dialog(msg, std::move(protocol), this);
    dialog.exec();
    if (msg) {
        GuiAppMgr::instance()->sendAddNewMessage(std::move(msg));
    }
}

void MainWindowWidget::sendRawMsgDialog(ToolsProtocolPtr protocol)
{
    ToolsMessagesList msgs;
    RawHexDataDialog dialog(msgs, std::move(protocol), this);
    dialog.exec();
    for (auto& msgPtr : msgs) {
        property::message::ToolsMsgRepeatCount().setTo(1, *msgPtr);
        GuiAppMgr::instance()->sendAddNewMessage(std::move(msgPtr));
    }
}

void MainWindowWidget::updateSendMsgDialog(
    ToolsMessagePtr msg,
    ToolsProtocolPtr protocol)
{
    assert(msg);
    MessageUpdateDialog dialog(msg, std::move(protocol), this);
    int result = dialog.exec();
    assert(msg);
    if (result != 0) {
        GuiAppMgr::instance()->sendUpdateMessage(std::move(msg));
    }
}

void MainWindowWidget::pluginsEditDialog()
{
    ToolsPluginMgr::ListOfPluginInfos selectedPlugins;
    PluginConfigDialog dialog(selectedPlugins, this);
    auto result = dialog.exec();
    if (result != QDialog::Accepted) {
        return;
    }

    bool applyResult = GuiAppMgr::instanceRef().applyNewPlugins(selectedPlugins);
    if (!applyResult) {
        QMessageBox::critical(
            this,
            tr("Plugins error occurred!"),
            tr("Failed to apply requested list of plugins."));
    }
}

void MainWindowWidget::displayErrorMsg(const QString& msg)
{
    QMessageBox::critical(
        this,
        QObject::tr("Error occurred!"),
        msg);
}

void MainWindowWidget::addMainToolbarAction(ActionPtr action)
{
    auto iter = std::find(m_customActions.begin(), m_customActions.end(), action);
    if (iter != m_customActions.end())
    {
        [[maybe_unused]] static constexpr bool Adding_action_second_time = false;
        assert(Adding_action_second_time);
        return;
    }

    assert(m_toolbar != nullptr);
    m_toolbar->addAction(action.get());
    m_customActions.push_back(action);
}

void MainWindowWidget::clearAllMainToolbarActions()
{
    for (auto& action : m_customActions) {
        m_toolbar->removeAction(action.get());
    }
    m_customActions.clear();
}

void MainWindowWidget::activeStateChanged(int state)
{
    auto castedState = static_cast<ActiveState>(state);
    if (castedState == ActiveState::Clear) {
        clearCustomToolbarActions();
    }
}

void MainWindowWidget::loadRecvMsgsDialog()
{
    auto result = loadMsgsDialog(false);
    auto& filename = std::get<0>(result);

    if (filename.isEmpty()) {
        return;
    }

    GuiAppMgr::instanceRef().recvLoadMsgsFromFile(filename);
}

void MainWindowWidget::saveRecvMsgsDialog()
{
    auto filename = saveMsgsDialog();
    if (filename.isEmpty()) {
        return;
    }

    GuiAppMgr::instance()->recvSaveMsgsToFile(filename);
}

void MainWindowWidget::loadSendMsgsDialog(bool askForClear)
{
    auto result = loadMsgsDialog(askForClear);
    auto& filename = std::get<0>(result);

    if (filename.isEmpty()) {
        return;
    }

    auto clear = std::get<1>(result);
    GuiAppMgr::instanceRef().sendLoadMsgsFromFile(clear, filename);
}

void MainWindowWidget::saveSendMsgsDialog()
{
    auto filename = saveMsgsDialog();
    if (filename.isEmpty()) {
        return;
    }

    GuiAppMgr::instance()->sendSaveMsgsToFile(filename);
}

void MainWindowWidget::msgCommentDialog(ToolsMessagePtr msg)
{
    assert(msg);
    MsgCommentDialog dialog(msg, this);
    dialog.resize(width() / 2, dialog.height());
    int result = dialog.exec();
    if (result != 0) {
        GuiAppMgr::instance()->msgCommentUpdated(std::move(msg));
    }
}

void MainWindowWidget::aboutInfo()
{
    static const QString AboutTxt(
        "<p>This application is a generic "
        "communication protocols analysis and visualization tool. " 
        "It is developed as a member project of the "
        "<a href=\"https://commschamp.github.io\">CommsChampion Ecosystem</a>."
        "</p>"
        "<p>"
        "The icons for this application were taken from: "
        "<a href=\"http://www.fatcow.com/free-icons\">FatCow</a>"
        "</p>");

    QMessageBox::information(this, tr("About"), AboutTxt);
}

void MainWindowWidget::recvFilterDialog(ToolsProtocolPtr protocol)
{
    auto* guiAppMgr = GuiAppMgr::instance();
    GuiAppMgr::FilteredMessages hiddenMessages = guiAppMgr->getFilteredMessages();
    MessagesFilterDialog dialog(hiddenMessages, std::move(protocol), this);
    dialog.exec();
    guiAppMgr->setFilteredMessages(std::move(hiddenMessages));
}

void MainWindowWidget::clearCustomToolbarActions()
{
    assert(m_toolbar != nullptr);
    for (auto& action : m_customActions) {
        m_toolbar->removeAction(action.get());
    }
    m_customActions.clear();
}

std::tuple<QString, bool> MainWindowWidget::loadMsgsDialog(bool askForClear)
{
    auto& msgsFileMgr = MsgFileMgrG::instanceRef();
    QString filename =
        QFileDialog::getOpenFileName(
            this,
            tr("Load Messages from File"),
            msgsFileMgr.getLastFile(),
            msgsFileMgr.getFilesFilter());

    bool clear = false;
    if (askForClear) {
        QMessageBox msgBox;
        msgBox.setText(
            tr("The list of messages is not empty.\n"
               "Do you want to CLEAR it first or APPEND new messages to it?"));
        auto* cancelButton = msgBox.addButton(tr("Cancel"), QMessageBox::ActionRole);
        assert(cancelButton != nullptr);
        auto* clearButton = msgBox.addButton(tr("Clear"), QMessageBox::ActionRole);
        assert(clearButton != nullptr);
        [[maybe_unused]] auto* appendButton = msgBox.addButton(tr("Append"), QMessageBox::ActionRole);
        assert(appendButton != nullptr);
        msgBox.setDefaultButton(clearButton);
        msgBox.setEscapeButton(cancelButton);
        assert(msgBox.clickedButton() == nullptr);
        msgBox.exec();
        assert(msgBox.clickedButton() != nullptr);
        if (msgBox.clickedButton() == cancelButton) {
            filename.clear();
        }
        else {
            clear = (msgBox.clickedButton() == clearButton);
        }
    }

    return std::make_tuple(std::move(filename), clear);
}

QString MainWindowWidget::saveMsgsDialog()
{
    auto& msgsFileMgr = MsgFileMgrG::instanceRef();
    return
        QFileDialog::getSaveFileName(
            this,
            tr("Save Messages to File"),
            msgsFileMgr.getLastFile(),
            msgsFileMgr.getFilesFilter());

}

}  // namespace cc_tools_qt
