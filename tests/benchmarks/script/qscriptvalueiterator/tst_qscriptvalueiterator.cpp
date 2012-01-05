/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qtest.h>
#include <QtScript>

class tst_QScriptValueIterator : public QObject
{
    Q_OBJECT

public:
    tst_QScriptValueIterator();
    virtual ~tst_QScriptValueIterator();

    void dataHelper();

private slots:
    void init();
    void cleanup();

    void hasNextAndNext();

    void constructAndNext_data();
    void constructAndNext();

    void name_data();
    void name();
    void scriptName_data();
    void scriptName();

    void value_data();
    void value();
    void setValue_data();
    void setValue();

    void flags();

    void iterateArrayAndConvertNameToIndex();
    void iterateArrayAndDoubleElements();
    void iterateArrayAndRemoveAllElements();
};

tst_QScriptValueIterator::tst_QScriptValueIterator()
{
}

tst_QScriptValueIterator::~tst_QScriptValueIterator()
{
}

void tst_QScriptValueIterator::init()
{
}

void tst_QScriptValueIterator::cleanup()
{
}

void tst_QScriptValueIterator::dataHelper()
{
    QTest::addColumn<QString>("code");
    QTest::newRow("{ foo: 123 }") << QString::fromLatin1("({ foo: 123 })");
    QTest::newRow("Math") << QString::fromLatin1("Math");
    QTest::newRow("Array.prototype") << QString::fromLatin1("Array.prototype");
    QTest::newRow("Global Object") << QString::fromLatin1("this");
    QTest::newRow("['foo']") << QString::fromLatin1("['foo']");
    QTest::newRow("array with 1000 elements")
        << QString::fromLatin1("(function() {"
                               "  var a = new Array;"
                               "  for (i = 0; i < 1000; ++i)"
                               "    a[i] = i;"
                               "  return a;"
                               "})()");
}

void tst_QScriptValueIterator::hasNextAndNext()
{
    QScriptEngine engine;
    QScriptValue object = engine.newObject();
    for (int i = 0; i < 2000; ++i)
        object.setProperty(i, i);
    QScriptValueIterator it(object);
    QBENCHMARK {
        for (int i = 0; i < 1000; ++i) {
            it.toFront();
            while (it.hasNext())
                it.next();
        }
    }
}

void tst_QScriptValueIterator::constructAndNext_data()
{
    dataHelper();
}

void tst_QScriptValueIterator::constructAndNext()
{
    QFETCH(QString, code);
    QScriptEngine engine;
    QScriptValue object = engine.evaluate(code);
    Q_ASSERT(object.isObject());

    QBENCHMARK {
        for (int i = 0; i < 100; ++i) {
            QScriptValueIterator it(object);
            it.next();
        }
    }
}

void tst_QScriptValueIterator::name_data()
{
    dataHelper();
}

void tst_QScriptValueIterator::name()
{
    QFETCH(QString, code);
    QScriptEngine engine;
    QScriptValue object = engine.evaluate(code);
    Q_ASSERT(object.isObject());

    QScriptValueIterator it(object);
    it.next();
    QBENCHMARK {
        for (int i = 0; i < 200000; ++i)
            it.name();
    }
}

void tst_QScriptValueIterator::scriptName_data()
{
    dataHelper();
}

void tst_QScriptValueIterator::scriptName()
{
    QFETCH(QString, code);
    QScriptEngine engine;
    QScriptValue object = engine.evaluate(code);
    Q_ASSERT(object.isObject());

    QScriptValueIterator it(object);
    it.next();
    QBENCHMARK {
        for (int i = 0; i < 50000; ++i)
            it.scriptName();
    }
}

void tst_QScriptValueIterator::value_data()
{
    dataHelper();
}

void tst_QScriptValueIterator::value()
{
    QFETCH(QString, code);
    QScriptEngine engine;
    QScriptValue object = engine.evaluate(code);
    Q_ASSERT(object.isObject());

    QScriptValueIterator it(object);
    it.next();
    QBENCHMARK {
        for (int i = 0; i < 50000; ++i)
            it.value();
    }
}

void tst_QScriptValueIterator::setValue_data()
{
    dataHelper();
}

void tst_QScriptValueIterator::setValue()
{
    QFETCH(QString, code);
    QScriptEngine engine;
    QScriptValue object = engine.evaluate(code);
    Q_ASSERT(object.isObject());

    QScriptValueIterator it(object);
    it.next();
    QScriptValue newValue(&engine, 456);
    QBENCHMARK {
        for (int i = 0; i < 50000; ++i)
            it.setValue(newValue);
    }
}

void tst_QScriptValueIterator::flags()
{
    QScriptEngine engine;
    QScriptValue object = engine.newObject();
    QScriptValue::PropertyFlags flags = flags;
    object.setProperty("foo", 123, QScriptValue::SkipInEnumeration | QScriptValue::ReadOnly | QScriptValue::Undeletable);
    QScriptValueIterator it(object);
    it.next();
    QBENCHMARK {
        for (int i = 0; i < 50000; ++i)
            it.flags();
    }
}

void tst_QScriptValueIterator::iterateArrayAndConvertNameToIndex()
{
    QScriptEngine engine;
    QScriptValue array = engine.newArray();
    for (int i = 0; i < 20000; ++i)
        array.setProperty(i, i);
    QBENCHMARK {
        QScriptValueIterator it(array);
        while (it.hasNext()) {
            it.next();
            it.scriptName().toArrayIndex();
        }
    }
}

void tst_QScriptValueIterator::iterateArrayAndDoubleElements()
{
    QScriptEngine engine;
    QScriptValue array = engine.newArray();
    for (int i = 0; i < 20000; ++i)
        array.setProperty(i, i);
    QBENCHMARK {
        QScriptValueIterator it(array);
        while (it.hasNext()) {
            it.next();
            it.setValue(QScriptValue(&engine, it.value().toNumber() * 2));
        }
    }
}

void tst_QScriptValueIterator::iterateArrayAndRemoveAllElements()
{
    QScriptEngine engine;
    QScriptValue array = engine.newArray();
    for (int i = 0; i < 20000; ++i)
        array.setProperty(i, i);
    QBENCHMARK {
        QScriptValueIterator it(array);
        while (it.hasNext()) {
            it.next();
            it.remove();
        }
    }
}

QTEST_MAIN(tst_QScriptValueIterator)
#include "tst_qscriptvalueiterator.moc"
