/*

                          Firewall Builder

                 Copyright (C) 2010 NetCitadel, LLC

  Author:  Roman Bovsunivskiy     a2k0001@gmail.com

  $Id: AddressRangeDialogTest.cpp 2723 2010-03-16 17:32:18Z a2k $

  This program is free software which we release under the GNU General Public
  License. You may redistribute and/or modify this program under the terms
  of that license as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  To get a copy of the GNU General Public License, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "AddressRangeDialogTest.h"

#include "../../../../config.h"
//#include "../../global.h"

#include <qapplication.h>
#include <qfile.h>
#include <qtextstream.h>
#include <QTest>
#include <iostream>

#include <QProcess>
#include <QRegExp>
#include <QDebug>

#include <QToolButton>
#include <QMessageBox>
#include <QWidget>
#include <QLineEdit>
#include "FWWindow.h"
#include "ProjectPanel.h"
#include "ObjectTreeView.h"
#include "ObjectTreeViewItem.h"
#include "ObjectEditor.h"
#include "FWObjectClipboard.h"
#include "TextEditWidget.h"
#include "fwbuilder/AddressRange.h"

using namespace std;
using namespace libfwbuilder;

QPoint findItemPos(ObjectTreeViewItem *item, ObjectTreeView *tree)
{
    for (int h=10; h<tree->height(); h+=1)
    {
        for (int w=75; w<tree->width(); w+=1)
        {
            if(tree->itemAt(w,h) == item)
                return QPoint(w, h);
        }
    }
    return QPoint(-1,-1);
}

void AddressRangeDialogTest::checkMessageBox()
{
    QVERIFY(app->activeModalWidget()->metaObject()->className() == QMessageBox().metaObject()->className());
    dynamic_cast<QMessageBox*>(app->activeModalWidget())->reject();
}

void AddressRangeDialogTest::editSelectedObject()
{
    mw = new FWWindow();
    mw->show();
    new FWObjectClipboard();
    mw->loadFile("test.fwb", false);
    QTest::qWait(100);
    QToolButton* newButton = mw->findChild<QToolButton*>("newButton");
    foreach(QAction *action, newButton->menu()->actions())
    {
        if (action->text() == "New Address Range")
        {
            action->trigger();
            break;
        }
    }
    QTest::qWait(100);
    QTreeWidgetItem *item = mw->getCurrentObjectTree()->findItems("Address Range", Qt::MatchRecursive | Qt::MatchExactly, 0).first();
    //QTreeWidgetItem *item = mw->getCurrentObjectTree()->findItems("Address Range", Qt::MatchRecursive | Qt::MatchExactly, 0).first();
    mw->getCurrentObjectTree()->setCurrentItem(item, 0, QItemSelectionModel::Clear | QItemSelectionModel::SelectCurrent);
    ObjectManipulator *om = dynamic_cast<ObjectManipulator*>(mw->getCurrentObjectTree()->parent()->parent());
    om->editSelectedObject();
    QTest::qWait(100);
    om->editSelectedObject();
    QLineEdit *rangeStart = mw->findChildren<QLineEdit*>("rangeStart").first();
    QLineEdit *rangeEnd = mw->findChildren<QLineEdit*>("rangeEnd").first();
    QLineEdit *objName = rangeStart->parent()->findChild<QLineEdit*>("obj_name");
    TextEditWidget *comment = rangeStart->parent()->parent()->findChild<TextEditWidget*>("comment");
    QVERIFY(comment != NULL);

    objName->clear();
    QTest::keyClicks(objName, "TestAddressRange");
    QTest::keyClick(objName, Qt::Key_Enter);
    QVERIFY (dynamic_cast<ObjectTreeViewItem*>(item)->getFWObject()->getName() == "TestAddressRange");

    QTest::qWait(100);

    comment->clear();
    QTest::mouseClick(comment, Qt::LeftButton);
    QTest::keyClicks(comment, "Test comment");
    QTest::mouseClick(comment, Qt::LeftButton);
    QTest::keyClick(comment, Qt::Key_Tab);
    QTest::qWait(100);
    QVERIFY (dynamic_cast<ObjectTreeViewItem*>(item)->getFWObject()->getComment() == "Test comment");


    rangeStart->clear();
    QTest::keyClicks(rangeStart, "1.1.1.1");
    QTest::keyClick(rangeStart, Qt::Key_Enter);
    QVERIFY (libfwbuilder::AddressRange::cast(dynamic_cast<ObjectTreeViewItem*>(item)->getFWObject())->getRangeStart().toString() == "1.1.1.1");
    QTest::qWait(100);

    rangeEnd->clear();
    QTest::keyClicks(rangeEnd, "1.1.1.10");
    QTest::keyClick(rangeEnd, Qt::Key_Enter);
    QVERIFY (libfwbuilder::AddressRange::cast(dynamic_cast<ObjectTreeViewItem*>(item)->getFWObject())->getRangeEnd().toString() == "1.1.1.10");


    QTimer::singleShot(1000, this, SLOT(checkMessageBox()));
    rangeStart->clear();
    QTest::keyClicks(rangeStart, "300.300.300.300");
    QTest::keyClick(rangeStart, Qt::Key_Enter);


    rangeStart->clear();
    QTest::keyClicks(rangeStart, "200.200.200.200");
    QTest::keyClick(rangeStart, Qt::Key_Enter);
    QTest::qWait(100);
    QTimer::singleShot(100, this, SLOT(checkMessageBox()));
    rangeEnd->clear();
    QTest::keyClicks(rangeEnd, "300.300.300.300");
    QTest::keyClick(rangeEnd, Qt::Key_Enter);

    QTest::qWait(1000);
}
