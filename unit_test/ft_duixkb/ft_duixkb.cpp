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



#include "ft_duixkb.h"
#include <DuiApplication>
#include <duiplainwindow.h>
#include <DuiNamespace>
#include <QDebug>
#include <QSignalSpy>
#include <QKeyEvent>
#include "duixkb.h"
//#include "duixkb_p.h"

void Ft_DuiXkb::initTestCase()
{
    // Avoid waiting if im server is not responding
    DuiApplication::setLoadDuiInputContext(false);

    static char *argv[2] = {(char *) "ft_duixkb", (char *) "-software"};
    static int argc = 2;
    app = new DuiApplication(argc, argv);
}

void Ft_DuiXkb::cleanupTestCase()
{
    delete app;
    app = 0;
}

void Ft_DuiXkb::init()
{
    m_subject = new DuiXkb();
}

void Ft_DuiXkb::cleanup()
{
    delete m_subject;
}

void Ft_DuiXkb::testLockModifiers()
{
    m_subject->lockModifiers(Qt::ShiftModifier);
    QCOMPARE(m_subject->isLatched(Qt::ShiftModifier), true);
    m_subject->lockModifiers(Qt::GroupSwitchModifier);
    QCOMPARE(m_subject->isLatched(Qt::GroupSwitchModifier), true);
    //lock both
    m_subject->unlockModifiers(Qt::ShiftModifier);
    m_subject->unlockModifiers(Qt::GroupSwitchModifier);
    QCOMPARE(m_subject->isLatched(Qt::ShiftModifier), false);
    QCOMPARE(m_subject->isLatched(Qt::GroupSwitchModifier), false);
    Qt::KeyboardModifiers modifiers = Qt::ShiftModifier | Qt::GroupSwitchModifier;
    m_subject->lockModifiers(modifiers);
    QCOMPARE(m_subject->isLatched(Qt::ShiftModifier), true);
    QCOMPARE(m_subject->isLatched(Qt::GroupSwitchModifier), true);
    m_subject->unlockModifiers(modifiers);
}

void Ft_DuiXkb::testUnlockModifiers()
{
    m_subject->lockModifiers(Qt::ShiftModifier);
    QCOMPARE(m_subject->isLatched(Qt::ShiftModifier), true);
    m_subject->unlockModifiers(Qt::ShiftModifier);
    QCOMPARE(m_subject->isLatched(Qt::ShiftModifier), false);
    m_subject->lockModifiers(Qt::GroupSwitchModifier);
    QCOMPARE(m_subject->isLatched(Qt::GroupSwitchModifier), true);
    m_subject->unlockModifiers(Qt::GroupSwitchModifier);
    QCOMPARE(m_subject->isLatched(Qt::GroupSwitchModifier), false);
}

QTEST_APPLESS_MAIN(Ft_DuiXkb);
