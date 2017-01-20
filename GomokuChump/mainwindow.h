#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include "playingboard.h"
#include "playscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void openFile();
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_exitB_clicked();

    void on_restartB_clicked();

    void on_pushButton_clicked();

private:
    bool playerTurn;
    playingBoard* board;
    Ui::MainWindow *ui;
    std::string path;

};

#endif // MAINWINDOW_H
