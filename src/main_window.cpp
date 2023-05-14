#include <string>
#include <QtWidgets>
#include <QLabel>

#include "main_window.hpp"

MainWindow::MainWindow() {
  setGeometry( 100, 100, 980, 780 );

  //mandelbrot zusammen bauen
  // pixmap erstellen
  // pixmap in große pixmap kopieren
  QLabel* mandelbrot_image = new QLabel(this);
  // pixmap in label speichern
  setCentralWidget(mandelbrot_image);
}

void MainWindow::closeEvent(QCloseEvent *event) 
{
  QMainWindow::closeEvent(event);
  if (event->isAccepted())
    emit closed();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
  QMainWindow::resizeEvent(event);
}

void MainWindow::close() {
  QMainWindow::close();    
}
