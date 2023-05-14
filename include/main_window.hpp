#ifndef MAIN_WINDOW_HPP 
#define MAIN_WINDOW_HPP

#include <QtWidgets/QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  void close();
signals:
  void closed();  
protected:
  void closeEvent(QCloseEvent *event) override;
  void resizeEvent(QResizeEvent* event) override;
};

#endif // MAIN_WINDOW_HPP
