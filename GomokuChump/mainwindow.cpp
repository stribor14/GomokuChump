#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openFile();

    board = new playingBoard(26, 26, path);

    ui->graphicsView->setScene(new playScene(board));
    ui->graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
}

void MainWindow::openFile()
{
    path = QFileDialog::getOpenFileName(this, tr("Open knowledge base"), "/home/mirko/Devel/EKSUS", tr("YAML Files (*.yaml)")).toStdString();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(board->win == false){
        int x = event->x() / 24;
        int y = event->y() / 24;
        if (x > 25 || y > 25) return;
        if(board->set(x, y, 1)){
            ui->graphicsView->scene()->update();
            if(board->win == false) board->doChumpsTurn();
            ui->graphicsView->scene()->update();
        }
    }
}


void MainWindow::on_exitB_clicked()
{
    close();
}

void MainWindow::on_restartB_clicked()
{
    delete board;
    board = new playingBoard(26,26, path);
    ((playScene*)ui->graphicsView->scene())->replaceBoard(board);
}

void MainWindow::on_pushButton_clicked()
{
    openFile();
    on_restartB_clicked();
}
