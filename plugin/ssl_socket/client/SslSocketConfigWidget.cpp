//
// Copyright 2023 - 2024 (C). Alex Robenko. All rights reserved.
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

#include "SslSocketConfigWidget.h"

#include <vector>
#include <utility>

#include <QtGlobal>
#include <QtWidgets/QFileDialog>

#include "comms/CompileControl.h"

namespace cc_tools_qt
{

namespace plugin
{

namespace ssl_socket
{

namespace client
{

namespace 
{

using PeerVerifyModeMap = std::vector<std::pair<QString, QSslSocket::PeerVerifyMode>>;
using SslProtocolMap = std::vector<std::pair<QString, QSsl::SslProtocol>>;
using EncodingFormatMap = std::vector<std::pair<QString, QSsl::EncodingFormat>>;
using KeyAlgorithmMap = std::vector<std::pair<QString, QSsl::KeyAlgorithm>>;

template <typename TMap>
void fillComboBox(const TMap& map, QComboBox& box)
{
    for (auto& elem : map) {
        box.addItem(elem.first, static_cast<int>(elem.second));
    }    
}

template <typename TMap, typename TVal>
void selectComboBoxText(const TMap& map, TVal value, QComboBox& box)
{
    for (auto& elem : map) {
        if (elem.second == value) {
            box.setCurrentText(elem.first);
            return;
        }
    }

    assert(false); // Should not happen
    box.setCurrentText(map.front().first);
}

const PeerVerifyModeMap& peerVerifyModeMap()
{
    static const PeerVerifyModeMap Map = {
        {"No", QSslSocket::VerifyNone},
        {"Yes", QSslSocket::VerifyPeer}
    };

    return Map;
}

COMMS_MSVC_WARNING_PUSH
COMMS_MSVC_WARNING_DISABLE(4996) // Disable deprecated values warning
const SslProtocolMap& sslProtocolMap()
{
    static const SslProtocolMap Map = {
        {"Any Secure", QSsl::SecureProtocols},
        {"Any Supported", QSsl::AnyProtocol},
        {"TLSv1.0", QSsl::TlsV1_0},
        {"TLSv1.0+", QSsl::TlsV1_0OrLater},
        {"TLSv1.1", QSsl::TlsV1_1},
        {"TLSv1.1+", QSsl::TlsV1_1OrLater},
        {"TLSv1.2", QSsl::TlsV1_2},
        {"TLSv1.2+", QSsl::TlsV1_2OrLater},
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        {"TLSv1.3", QSsl::TlsV1_3},
        {"TLSv1.3+", QSsl::TlsV1_3OrLater},
#endif        
        {"DTLSv1.0", QSsl::DtlsV1_0},
        {"DTLSv1.0+", QSsl::DtlsV1_0OrLater},
        {"DTLSv1.2", QSsl::DtlsV1_2},
        {"DTLSv1.2+", QSsl::DtlsV1_2OrLater},
    };

    return Map;
}
COMMS_MSVC_WARNING_POP

const EncodingFormatMap& encodingFormatMap()
{
    static const EncodingFormatMap Map = {
        {"PEM", QSsl::Pem},
        {"DER", QSsl::Der}
    };

    return Map;
}

const KeyAlgorithmMap& keyAlgorithmMap()
{
    static const KeyAlgorithmMap Map = {
        {"RSA", QSsl::Rsa},
        {"DSA", QSsl::Dsa},
        {"EC", QSsl::Ec},
        {"DH", QSsl::Dh},
    };

    return Map;
}

} // namespace 
    

SslSocketConfigWidget::SslSocketConfigWidget(
    SslSocket& socket,
    QWidget* parentObj)
  : Base(parentObj),
    m_socket(socket)
{
    m_ui.setupUi(this);

    m_ui.m_portSpinBox->setRange(
        1,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    m_ui.m_hostLineEdit->setText(m_socket.getHost());

    m_ui.m_portSpinBox->setValue(
        static_cast<int>(m_socket.getPort()));

    m_ui.m_caDirLineEdit->setText(m_socket.getCaDir());
    fillComboBox(encodingFormatMap(), *m_ui.m_caDirFormatComboBox);
    selectComboBoxText(encodingFormatMap(), m_socket.getCaDirFormat(), *m_ui.m_caDirFormatComboBox);     

    m_ui.m_caFileLineEdit->setText(m_socket.getCaFile());
    fillComboBox(encodingFormatMap(), *m_ui.m_caFileFormatComboBox);
    selectComboBoxText(encodingFormatMap(), m_socket.getCaFileFormat(), *m_ui.m_caFileFormatComboBox);     

    fillComboBox(peerVerifyModeMap(), *m_ui.m_verifyHostComboBox);
    selectComboBoxText(peerVerifyModeMap(), m_socket.getVerifyMode(), *m_ui.m_verifyHostComboBox);

    m_ui.m_verifyNameLineEdit->setText(m_socket.getVerifyName());

    fillComboBox(sslProtocolMap(), *m_ui.m_protocolComboBox);    
    selectComboBoxText(sslProtocolMap(), m_socket.getSslProtocol(), *m_ui.m_protocolComboBox);

    m_ui.m_certLineEdit->setText(m_socket.getCertFile());
    fillComboBox(encodingFormatMap(), *m_ui.m_certFormatComboBox);
    selectComboBoxText(encodingFormatMap(), m_socket.getCertFormat(), *m_ui.m_certFormatComboBox);     

    m_ui.m_privKeyLineEdit->setText(m_socket.getPrivKeyFile());
    fillComboBox(keyAlgorithmMap(), *m_ui.m_privKeyAlgComboBox);
    selectComboBoxText(keyAlgorithmMap(), m_socket.getPrivKeyAlg(), *m_ui.m_privKeyAlgComboBox);     
    fillComboBox(encodingFormatMap(), *m_ui.m_privKeyFormatComboBox);
    selectComboBoxText(encodingFormatMap(), m_socket.getPrivKeyFormat(), *m_ui.m_privKeyFormatComboBox);     

    m_ui.m_privKeyPassLineEdit->setText(m_socket.getPrivKeyPass());

    connect(
        m_ui.m_hostLineEdit,  &QLineEdit::textChanged,
        this, &SslSocketConfigWidget::hostValueChanged);

    connect(
        m_ui.m_portSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(portValueChanged(int)));

    connect(
        m_ui.m_caDirLineEdit, &QLineEdit::textChanged,
        this, &SslSocketConfigWidget::caDirValueChanged);  

    connect(
        m_ui.m_caDirToolButton, SIGNAL(clicked()),
        this, SLOT(caDirSelectClicked()));

    connect(
        m_ui.m_caDirFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(caDirFormatIndexChanged(int)));     

    connect(
        m_ui.m_caFileLineEdit, &QLineEdit::textChanged,
        this, &SslSocketConfigWidget::caFileValueChanged);  

    connect(
        m_ui.m_caFileToolButton, SIGNAL(clicked()),
        this, SLOT(caFileSelectClicked()));

    connect(
        m_ui.m_caFileFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(caFileFormatIndexChanged(int)));         

    connect(
        m_ui.m_verifyHostComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(verifyHostIndexChanged(int)));     

    connect(
        m_ui.m_verifyNameLineEdit, &QLineEdit::textChanged,
        this, &SslSocketConfigWidget::verifyNameValueChanged);          

    connect(
        m_ui.m_protocolComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(sslProtocolIndexChanged(int)));                   

    connect(
        m_ui.m_certLineEdit,  &QLineEdit::textChanged,
        this, &SslSocketConfigWidget::certValueChanged);

    connect(
        m_ui.m_certToolButton, SIGNAL(clicked()),
        this, SLOT(certSelectClicked()));         

    connect(
        m_ui.m_certFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(certFormatIndexChanged(int)));     

    connect(
        m_ui.m_privKeyLineEdit,  &QLineEdit::textChanged,
        this, &SslSocketConfigWidget::privKeyValueChanged);

    connect(
        m_ui.m_privKeyToolButton, SIGNAL(clicked()),
        this, SLOT(privKeySelectClicked()));        

    connect(
        m_ui.m_privKeyAlgComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(privKeyAlgIndexChanged(int)));             

    connect(
        m_ui.m_privKeyFormatComboBox,  SIGNAL(currentIndexChanged(int)),
        this, SLOT(privKeyFormatIndexChanged(int)));

    connect(
        m_ui.m_privKeyPassLineEdit,  &QLineEdit::textChanged,
        this, &SslSocketConfigWidget::privKeyPassValueChanged);
}

SslSocketConfigWidget::~SslSocketConfigWidget() noexcept = default;

void SslSocketConfigWidget::hostValueChanged(const QString& value)
{
    m_socket.setHost(value);
}

void SslSocketConfigWidget::portValueChanged(int value)
{
    m_socket.setPort(static_cast<PortType>(value));
}

void SslSocketConfigWidget::caDirValueChanged(const QString& value)
{
    m_socket.setCaDir(value);
}

void SslSocketConfigWidget::caDirSelectClicked()
{
    QString dir = 
        QFileDialog::getExistingDirectory(
            this, 
            tr("Select CA Directory"),
            m_socket.getCaDir(),
            QFileDialog::ShowDirsOnly);

    if (!dir.isEmpty()) {
        m_ui.m_caDirLineEdit->setText(dir);
    }
}

void SslSocketConfigWidget::caDirFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setCaDirFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_caDirFormatComboBox->currentData().toInt()));
}

void SslSocketConfigWidget::caFileValueChanged(const QString& value)
{
    m_socket.setCaFile(value);
}

void SslSocketConfigWidget::caFileSelectClicked()
{
    QString file = 
        QFileDialog::getOpenFileName(
            this, 
            tr("Select CA File"),
            m_socket.getCaFile());

    if (!file.isEmpty()) {
        m_ui.m_caFileLineEdit->setText(file);
    }
}

void SslSocketConfigWidget::caFileFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setCaFileFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_caFileFormatComboBox->currentData().toInt()));
}

void SslSocketConfigWidget::verifyHostIndexChanged([[maybe_unused]] int value)
{
    m_socket.setVerifyMode(static_cast<QSslSocket::PeerVerifyMode>(m_ui.m_verifyHostComboBox->currentData().toInt()));
}

void SslSocketConfigWidget::verifyNameValueChanged(const QString& value)
{
    m_socket.setVerifyName(value);
}

void SslSocketConfigWidget::sslProtocolIndexChanged([[maybe_unused]] int value)
{
    m_socket.setSslProtocol(static_cast<QSsl::SslProtocol>(m_ui.m_protocolComboBox->currentData().toInt()));
}

void SslSocketConfigWidget::certValueChanged(const QString& value)
{
    m_socket.setCertFile(value);
}

void SslSocketConfigWidget::certSelectClicked()
{
    QString file = 
        QFileDialog::getOpenFileName(
            this, 
            tr("Select Local Certificate File"),
            m_socket.getCertFile());

    if (!file.isEmpty()) {
        m_ui.m_certLineEdit->setText(file);
    }
}

void SslSocketConfigWidget::certFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setCertFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_certFormatComboBox->currentData().toInt()));
}

void SslSocketConfigWidget::privKeyValueChanged(const QString& value)
{
    m_socket.setPrivKeyFile(value);
}

void SslSocketConfigWidget::privKeySelectClicked()
{
    QString file = 
        QFileDialog::getOpenFileName(
            this, 
            tr("Select Private Key File"),
            m_socket.getPrivKeyFile());

    if (!file.isEmpty()) {
        m_ui.m_privKeyLineEdit->setText(file);
    }
}

void SslSocketConfigWidget::privKeyAlgIndexChanged([[maybe_unused]] int value)
{
    m_socket.setPrivKeyAlg(static_cast<QSsl::KeyAlgorithm>(m_ui.m_privKeyAlgComboBox->currentData().toInt()));
}

void SslSocketConfigWidget::privKeyFormatIndexChanged([[maybe_unused]] int value)
{
    m_socket.setPrivKeyFormat(static_cast<QSsl::EncodingFormat>(m_ui.m_privKeyFormatComboBox->currentData().toInt()));
}

void SslSocketConfigWidget::privKeyPassValueChanged(const QString& value)
{
    m_socket.setPrivKeyPass(value);
}

}  // namespace client

}  // namespace ssl_socket

}  // namespace plugin

}  // namespace cc_tools_qt


