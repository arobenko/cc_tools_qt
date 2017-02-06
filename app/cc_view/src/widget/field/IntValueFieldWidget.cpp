//
// Copyright 2015 - 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "IntValueFieldWidget.h"

#include <cassert>

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtWidgets/QVBoxLayout>
CC_ENABLE_WARNINGS()

#include "comms_champion/property/field.h"

#include "ShortIntValueFieldWidget.h"
#include "LongIntValueFieldWidget.h"
#include "ScaledIntValueFieldWidget.h"

namespace comms_champion
{

IntValueFieldWidget::IntValueFieldWidget(WrapperPtr wrapper, QWidget* parentObj)
  : Base(parentObj),
    m_wrapper(std::move(wrapper))
{
}

IntValueFieldWidget::~IntValueFieldWidget()
{
    m_childWidget.release();
}

void IntValueFieldWidget::refreshImpl()
{
    if (m_childWidget) {
        m_childWidget->refresh();
    }
}

void IntValueFieldWidget::editEnabledUpdatedImpl()
{
    if (m_childWidget) {
        m_childWidget->setEditEnabled(isEditEnabled());
    }
}

void IntValueFieldWidget::updatePropertiesImpl(const QVariantMap& props)
{
    assert(m_wrapper);
    assert(!m_childWidget);
    if (property::field::IntValue(props).hasScaledDecimals()) {
        m_childWidget.reset(new ScaledIntValueFieldWidget(std::move(m_wrapper)));
    }
    else if (m_wrapper->isShortInt()) {
        m_childWidget.reset(new ShortIntValueFieldWidget(std::move(m_wrapper)));
    }
    else {
        m_childWidget.reset(new LongIntValueFieldWidget(std::move(m_wrapper)));
    }

    auto* childLayout = new QVBoxLayout();
    childLayout->addWidget(m_childWidget.get());
    childLayout->setContentsMargins(0, 0, 0, 0);
    childLayout->setSpacing(0);
    setLayout(childLayout);
    m_childWidget->updateProperties(props);
    m_childWidget->setEditEnabled(isEditEnabled());

    connect(
        m_childWidget.get(), SIGNAL(sigFieldUpdated()),
        this, SIGNAL(sigFieldUpdated()));
}

}  // namespace comms_champion

