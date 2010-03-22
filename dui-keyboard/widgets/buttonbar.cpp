/* * This file is part of dui-keyboard *
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * Contact: Nokia Corporation (directui@nokia.com)
 *
 * If you have questions regarding the use of this file, please contact
 * Nokia at directui@nokia.com.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * and appearing in the file LICENSE.LGPL included in the packaging
 * of this file.
 */



#include "buttonbar.h"

#include <DuiButton>

#include <QDebug>
#include <QGraphicsLinearLayout>
#include <QPixmap>

// No use for us but prevents DuiClassFactory bitching about it.
#include <DuiWidgetCreator>
DUI_REGISTER_WIDGET_NO_CREATE(ButtonBar)

ButtonBar::ButtonBar(bool useDividers, QGraphicsItem *parent)
    : DuiStylableWidget(parent),
      mainLayout(*new QGraphicsLinearLayout(Qt::Horizontal, this)),
      useDividers(useDividers)
{
    mainLayout.setSpacing(0); // Spacing is handled by dividers.
    mainLayout.setContentsMargins(0, 0, 0, 0);

    // Update style by calling styleChanged(). This if for widgets that don't have
    // object name set.
    styleChanged();
}

ButtonBar::~ButtonBar()
{
}

int ButtonBar::count() const
{
    return buttons.count();
}

void ButtonBar::insert(int index, DuiButton *button)
{
    Q_ASSERT(button);

    if (index < 0 || index > count()) {
        qWarning() << "Invalid index given when inserting a button to ButtonBar.";
        return;
    }

    buttons.insert(index, button);

    mainLayout.insertItem(index, button);
    mainLayout.setAlignment(button, Qt::AlignVCenter); // In case we have buttons that differs in height.
}

void ButtonBar::append(DuiButton *button)
{
    insert(count(), button);
}

void ButtonBar::remove(DuiButton *button)
{
    mainLayout.removeItem(button);
    buttons.removeOne(button);
}

void ButtonBar::clear()
{
    buttons.clear();
    while (mainLayout.count() > 0) {
        mainLayout.removeAt(0);
    }
}

DuiButton *ButtonBar::buttonAt(int index) const
{
    DuiButton *button = 0;
    if (index >= 0 && index < count()) {
        button = buttons.at(index);
    }
    return button;
}

bool ButtonBar::contains(const DuiButton *button) const
{
    return buttons.contains(const_cast<DuiButton *>(button));
}

int ButtonBar::indexOf(const DuiButton *button) const
{
    return buttons.indexOf(const_cast<DuiButton *>(button));
}

void ButtonBar::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    // Stop propagating
}

void ButtonBar::styleChanged()
{
    setContentsMargins(style()->paddingLeft(), style()->paddingTop(),
                       style()->paddingRight(), 0);
}

