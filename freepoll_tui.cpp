#include <final/final.h>

class MainWindow : public finalcut::FWindow {
public:
  MainWindow( finalcut::FWidget *parent = nullptr );
  ~MainWindow();

  void draw() override;
  void onClose( finalcut::FCloseEvent *evt ) override;
  void onShow( finalcut::FShowEvent *evt ) override;
  void onKeyPress( finalcut::FKeyEvent *evt ) override;
  void adjustSize() override;

private:
  void layoutChildren();
};


MainWindow::MainWindow( finalcut::FWidget *parent )
  : finalcut::FWindow( parent ) {
}

MainWindow::~MainWindow() {
}

void MainWindow::draw() {
  //finalcut::FWindow::draw();
  //print() << finalcut::FPoint{2, 2} << "Hello";

  finalcut::FSize win_size = getSize();
  int x = win_size.getWidth() / 2 - 3;
  int y = win_size.getHeight() / 2;

  print() << finalcut::FPoint{x, y} << "Hello";
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

void MainWindow::adjustSize() {
  setSize({ getDesktopWidth(), getDesktopHeight() });
  layoutChildren();
  redraw();
}

void MainWindow::layoutChildren() {
  // TODO
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
