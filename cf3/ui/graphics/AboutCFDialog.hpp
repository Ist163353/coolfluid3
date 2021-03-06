// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_ui_Graphics_AboutCFDialog_hpp
#define cf3_ui_Graphics_AboutCFDialog_hpp

////////////////////////////////////////////////////////////////////////////////

#include <QDialog>
#include <QList>

#include "ui/graphics/LibGraphics.hpp"

class QFormLayout;
class QLabel;
class QPushButton;
class QVBoxLayout;
class QWidget;

////////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace ui {
namespace graphics {

////////////////////////////////////////////////////////////////////////////////

  /// Builds and shows up the "About COOLFLuiD" dialog.
  /// This dialog displays some information about COOLFluiD, such as the kernel
  /// version, the operating system type,...
  /// @todo should display the license, CF logo, icons credits...
  /// @author Quentin Gasper
  class Graphics_API AboutCFDialog : public QDialog
  {

  public:

    /// Constructor.
    /// @param parent The parent widget.
    AboutCFDialog(QWidget * parent = nullptr);

    /// Destructor.
    /// Frees all allocated memory.
    ~AboutCFDialog();

  private: // data

    /// Main layout
    QVBoxLayout * m_main_layout;

    /// "OK" button
    QPushButton * m_bt_ok;

    /// Layout that contain the information.
    QFormLayout * m_info_layout;

    /// Label for Qwt.
    QLabel * m_lab_qwt;

  }; // class AboutCFDialog

  ///////////////////////////////////////////////////////////////////////////////

} // Graphics
} // ui
} // cf3

////////////////////////////////////////////////////////////////////////////////

#endif // cf3_ui_Graphics_AboutCFDialo.hpp
