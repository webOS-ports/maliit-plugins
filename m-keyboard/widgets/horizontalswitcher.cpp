/* * This file is part of meego-keyboard *
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


#include "horizontalswitcher.h"
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsScene>
#include <QDebug>

namespace
{
    const int SwitchDuration = 500;
    const int SwitchFrames = 300;
}

HorizontalSwitcher::HorizontalSwitcher(QGraphicsItem *parent) :
    QGraphicsWidget(parent),
    currentIndex(-1),
    animTimeLine(SwitchDuration),
    loopingEnabled(false)
{
    setFlag(QGraphicsItem::ItemHasNoContents); // doesn't paint itself anything
    setObjectName("HorizontalSwitcher");

    animTimeLine.setFrameRange(0, SwitchFrames);

    enterAnim.setTimeLine(&animTimeLine);
    leaveAnim.setTimeLine(&animTimeLine);

    connect(&animTimeLine, SIGNAL(finished()), this, SLOT(finishAnimation()));
}

HorizontalSwitcher::~HorizontalSwitcher()
{
    if (isRunning())
        finishAnimation();

    // Delete all widgets that were not removed with removeWidget().
    qDeleteAll(slides);
    slides.clear();
}

void HorizontalSwitcher::switchTo(SwitchDirection direction)
{
    if (isRunning())
        finishAnimation();

    if (slides.count() <= 1)
        return;

    int newIndex;
    if (direction == Right) {
        if (!loopingEnabled && currentIndex == slides.count() - 1)
            return;
        newIndex = (currentIndex + 1) % slides.count();
    } else { // Left
        if (!loopingEnabled && currentIndex == 0)
            return;
        newIndex = currentIndex - 1;
        if (newIndex < 0)
            newIndex += slides.count();
    }

    QGraphicsWidget *currentWidget = slides.at(currentIndex);
    QGraphicsWidget *nextWidget = slides.at(newIndex);

    // Current item is about to leave
    leaveAnim.setItem(currentWidget);

    // New item is about to enter
    enterAnim.setItem(nextWidget);

    // Try to fit current size.
    nextWidget->resize(size());

    if (direction == Right) {
        // Set the new item to the right of this.
        nextWidget->setPos(this->size().width(), 0.0);
        enterAnim.setPosAt(0.0, nextWidget->pos());
        enterAnim.setPosAt(1.0, QPointF(0.0, 0.0));
        leaveAnim.setPosAt(0.0, currentWidget->pos());
        leaveAnim.setPosAt(1.0, QPointF(-currentWidget->size().width(), 0.0));
    } else {
        // Set the new item to the left of this.
        nextWidget->setPos(-nextWidget->size().width(), 0.0);
        enterAnim.setPosAt(0.0, nextWidget->pos());
        enterAnim.setPosAt(1.0, QPointF(0.0, 0.0));
        leaveAnim.setPosAt(0.0, currentWidget->pos());
        leaveAnim.setPosAt(1.0, QPointF(this->size().width(), 0.0));
    }

    nextWidget->show();

    emit switchStarting(currentIndex, newIndex);
    emit switchStarting(currentWidget, nextWidget);

    currentIndex = newIndex;
    animTimeLine.start();
}

bool HorizontalSwitcher::isAtBoundary(SwitchDirection direction) const
{
    if (direction == Right) {
        if (currentIndex == slides.count() - 1) {
            return true;
        }
    } else { // Left
        if (currentIndex == 0) {
            return true;
        }
    }

    return false;
}

void HorizontalSwitcher::setCurrent(QGraphicsWidget *widget)
{
    Q_ASSERT(widget && slides.contains(widget));
    setCurrent(slides.indexOf(widget));
}

void HorizontalSwitcher::setCurrent(int index)
{
    if (isValidIndex(index) && index != currentIndex) {
        int oldIndex = -1;
        QGraphicsWidget *old = 0;

        if (isValidIndex(currentIndex)) {
            oldIndex = currentIndex;
            old = slides.at(currentIndex);
        }

        currentIndex = index;

        QGraphicsWidget *widget = slides.at(index);
        widget->setPos(0, 0);
        widget->resize(size());
        widget->show();

        // Ultimately might lead to a reaction map update in MKeyboardHost,
        // has no other purpose:
        emit switchDone(old, widget);

        updateGeometry();

        if (old) {
            old->hide();
        }
    }
}

int HorizontalSwitcher::current() const
{
    return (slides.isEmpty() ? -1 : currentIndex);
}

QGraphicsWidget *HorizontalSwitcher::currentWidget() const
{
    QGraphicsWidget *widget = NULL;
    if (current() >= 0) {
        widget = slides.at(currentIndex);
    }
    return widget;
}

QGraphicsWidget *HorizontalSwitcher::widget(int index)
{
    QGraphicsWidget *widget = NULL;
    if (index >= 0 && index < slides.count()) {
        widget = slides.at(index);
    }
    return widget;
}

int HorizontalSwitcher::count() const
{
    return slides.count();
}

bool HorizontalSwitcher::isRunning() const
{
    return (animTimeLine.state() == QTimeLine::Running);
}

void HorizontalSwitcher::setLooping(bool enable)
{
    loopingEnabled = enable;
}

void HorizontalSwitcher::setDuration(int ms)
{
    animTimeLine.setDuration(ms);
    animTimeLine.setFrameRange(0, ms * SwitchFrames / SwitchDuration);
}

void HorizontalSwitcher::addWidget(QGraphicsWidget *widget)
{
    if (widget) {
        widget->setParentItem(this);
        widget->setPreferredWidth(size().width());

        slides.append(widget);
        widget->hide();

        // HS was empty before, this was the first widget added:
        if (slides.size() == 1) {
            setCurrent(0);
        }
    }
}

void HorizontalSwitcher::removeAll()
{
    foreach(QGraphicsWidget * slide, slides) {
        slide->setParentItem(0);

        if (slide->scene()) {
            slide->scene()->removeItem(slide);
        }
    }

    slides.clear();
    currentIndex = -1;
    updateGeometry();
}


void HorizontalSwitcher::deleteAll()
{
    qDeleteAll(slides);
    slides.clear();
    currentIndex = -1;
    updateGeometry();
}


void HorizontalSwitcher::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget *widget = currentWidget();
    if (widget) {
        widget->resize(event->newSize());
    }
}


QSizeF HorizontalSwitcher::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    // return the size hint of the currently visible widget
    QGraphicsWidget *widget = currentWidget();
    QSizeF hint;

    if (widget) {
        hint = widget->effectiveSizeHint(which, constraint);
    } else {
        hint = QGraphicsWidget::sizeHint(which, constraint);
    }

    return hint;
}


void HorizontalSwitcher::finishAnimation()
{
    int oldIndex = -1;

    // Hide old item
    QGraphicsWidget *old = static_cast<QGraphicsWidget *>(leaveAnim.item());
    if (old) {
        oldIndex = slides.indexOf(old);
        old->hide();
    }

    // Clear transformations
    leaveAnim.clear();
    enterAnim.clear();

    animTimeLine.stop();

    // Discard cached sizehint info before telling that the switch is done.
    updateGeometry();

    emit switchDone(oldIndex, currentIndex);
    emit switchDone(old, slides.at(currentIndex));
}

bool HorizontalSwitcher::isValidIndex(int index) const
{
    return (index >= 0 && index < slides.size());
}
