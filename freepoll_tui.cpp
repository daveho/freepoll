#include <final/final.h>

class MainWindow : public finalcut::FWindow {
public:
  MainWindow( finalcut::FWidget *parent = nullptr );
  ~MainWindow();

  void draw();
  void onClose( finalcut::FCloseEvent *evt );
  void onShow( finalcut::FShowEvent *evt );
  void onKeyPress( finalcut::FKeyEvent *evt );

private:
};


MainWindow::MainWindow( finalcut::FWidget *parent )
  : finalcut::FWindow( parent ) {
  //setModal( true );
  //setResizeable( false );
  //setMinimizable( false );
  //setMinimumSize( getSize() );
  //unsetBorder();
  //activateDialog();
}

MainWindow::~MainWindow() {
}

void MainWindow::draw() {
  //finalcut::FWindow::draw();
  print() << finalcut::FPoint{2, 2} << "Hello";
}

void MainWindow::onClose( finalcut::FCloseEvent *evt ) {
  evt->accept();
}

void MainWindow::onShow( finalcut::FShowEvent *evt ) {
}

void MainWindow::onKeyPress( finalcut::FKeyEvent *evt ) {
  if ( !evt )
    return;

  if ( evt->key() == finalcut::FKey('q') ) {
    close();
    evt->accept();
  } else {
    finalcut::FWindow::onKeyPress( evt );
  }
}

int main( int argc, char **argv ) {
  finalcut::FApplication app { argc, argv };
  finalcut::FVTerm::setNonBlockingRead();

  MainWindow main_win { &app };
  //main_win.setGeometry( finalcut::FPoint{4, 4}, finalcut::FSize{10, 5} );
  main_win.setGeometry( finalcut::FPoint{1, 1}, app.getSize() );

  finalcut::FWidget::setMainWidget( &main_win );

  main_win.show();

  finalcut::FApplication::setDarkTheme();
  app.resetColors();
  app.redraw();

  return app.exec();
}
