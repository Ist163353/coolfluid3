// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_GUI_Client_uTests_GraphicalUintTest_hpp
#define cf3_GUI_Client_uTests_GraphicalUintTest_hpp

#include <QObject>

class QDoubleSpinBox;

////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace UI {

namespace Graphics { class GraphicalInt; }

namespace GraphicsTest {

//////////////////////////////////////////////////////////////////////////

class GraphicalUintTest : public QObject
{
  Q_OBJECT

private slots:

  void initTestCase();

  void test_constructor();

  void test_setValue();

  void test_value();

  void test_signalEmmitting();

  void test_valueString();

  void test_isModified();

private:

  QDoubleSpinBox * findSpinBox(const Graphics::GraphicalInt* value);

};

//////////////////////////////////////////////////////////////////////////

} // GraphicsTest
} // UI
} // cf3

////////////////////////////////////////////////////////////////////////////

#endif // CF3_GUI_Client_uTests_GraphicalUintTest_hpp
