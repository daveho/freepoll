#include <final/final.h>

class MainWindow : public finalcut::FDialog {
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
  : finalcut::FDialog( parent ) {
  FDialog::setStatusbarMessage( "Press Q to quit" );

  activateDialog();
}

MainWindow::~MainWindow() {
}

void MainWindow::draw() {
  finalcut::FDialog::draw();
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
    finalcut::FDialog::onKeyPress( evt );
  }
}

int main( int argc, char **argv ) {
  finalcut::FApplication app { argc, argv };
  finalcut::FVTerm::setNonBlockingRead();
  app.setDarkTheme();

  MainWindow main_win { &app };
  main_win.setGeometry( finalcut::FPoint{4, 4}, finalcut::FSize{10, 5} );

  finalcut::FWidget::setMainWidget( &main_win );

  main_win.show();
  return app.exec();
}
