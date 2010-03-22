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



#include "bm_symbols.h"
#include "duivirtualkeyboard.h"
#include "duivirtualkeyboardstyle.h"
#include "keyboarddata.h"
#include "keybuttonarea.h"

#include <DuiApplication>
#include <DuiTheme>

#include <QDir>

void Bm_Symbols::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "bm_symbols" };

    // Avoid waiting if im server is not responding
    DuiApplication::setLoadDuiInputContext(false);
    app = new DuiApplication(argc, app_name);

    DuiTheme::instance()->loadCSS("/usr/share/dui/virtual-keyboard/css/864x480.css");
    style = new DuiVirtualKeyboardStyleContainer;
    style->initialize("DuiVirtualKeyboard", "DuiVirtualKeyboardView", 0);
}

void Bm_Symbols::cleanupTestCase()
{
    delete style;
    style = 0;
    delete app;
    app = 0;
}

void Bm_Symbols::init()
{
    subject = 0;
    keyboard = 0;
}

void Bm_Symbols::cleanup()
{
    delete subject;
    subject = 0;
    delete keyboard;
    keyboard = 0;
}

void Bm_Symbols::benchmarkDraw_data()
{
    QDir dir("/usr/share/dui/virtual-keyboard/layouts/");
    QStringList filters;
    QFileInfoList files;
    QFileInfo info;

    QTest::addColumn<QString>("filename");
    filters << "symbols.xml";
    files = dir.entryInfoList(filters);
    for (int n = files.count() - 1; n >= 0; --n) {
        info = files.at(n);
        QTest::newRow(info.fileName().toLatin1().constData()) << info.fileName();
    }
}

void Bm_Symbols::benchmarkDraw()
{
    QFETCH(QString, filename);
    QImage image(QSize(864, 480), QImage::Format_ARGB32_Premultiplied);
    QPainter painter;

    QVERIFY(painter.begin(&image));
    keyboard = new KeyboardData;
    QVERIFY(keyboard->loadNokiaKeyboard(filename));

    QSharedPointer<const LayoutSection> section = keyboard->layout(LayoutData::General, Dui::Landscape)->section(LayoutData::symbolsSymSection);
    QVERIFY(!section.isNull());

    subject = new SingleWidgetButtonArea(style, section);

    QBENCHMARK {
        subject->paint(&painter, 0, 0);
    }
    painter.end();
}

void Bm_Symbols::benchmarkLoadXML_data()
{
    QDir dir("/usr/share/dui/virtual-keyboard/layouts/");
    QStringList filters;
    QFileInfoList files;
    QFileInfo info;

    QTest::addColumn<QString>("filename");
    filters << "symbols.xml";
    files = dir.entryInfoList(filters);
    for (int n = files.count() - 1; n >= 0; --n) {
        info = files.at(n);
        QTest::newRow(info.fileName().toLatin1().constData()) << info.fileName();
    }
}

void Bm_Symbols::benchmarkLoadXML()
{
    QFETCH(QString, filename);

    QBENCHMARK {
        keyboard = new KeyboardData;
        QVERIFY(keyboard->loadNokiaKeyboard(filename));
        delete keyboard;
        keyboard = 0;
    }
}

QTEST_APPLESS_MAIN(Bm_Symbols);
