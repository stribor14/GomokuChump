#include "playscene.h"
#include <cmath>
#include <iostream>
playScene::playScene(playingBoard* board) : board_(board)
{

}

void playScene::replaceBoard(playingBoard *board)
{
    board_ = board;
    update();
}

void playScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    QPen pen[3];
    QBrush brush[3];
    pen[0].setColor(Qt::lightGray);
    painter->setPen(pen[0]);

    for(int k=24; k < 624; k+=24){
        painter->drawLine(QPoint(0,k),QPoint(624,k));
        painter->drawLine(QPoint(k,0),QPoint(k,624));
    }

    pen[0].setColor(Qt::transparent);
    pen[1].setColor(Qt::blue);
    pen[2].setColor(Qt::red);
    brush[0].setColor(Qt::transparent);
    brush[1].setColor(Qt::blue);
    brush[2].setColor(Qt::red);
    brush[0].setStyle(Qt::SolidPattern);
    brush[1].setStyle(Qt::SolidPattern);
    brush[2].setStyle(Qt::SolidPattern);

    for(int x=0; x<26; x++)
        for(int y=0; y<26; y++){
            painter->setPen(pen[board_->get(x,y)]);
            painter->setBrush(brush[board_->get(x,y)]);
            painter->drawEllipse(QPoint(x*24+12, y*24+12), 10, 10);
        }

    if(board_->win == true){
        int x1 = board_->win.p1.first;
        int y1 = board_->win.p1.second;
        int x2 = board_->win.p2.first;
        int y2 = board_->win.p2.second;
        painter->setPen(pen[board_->get(x1, y1)]);
        painter->setBrush(brush[board_->get(x1, y1)]);
        int angle = round(atan2(y2 - y1, x2 - x1)*180 / (atan(1)*4));
        double ofx = angle == 0 ? 0 : angle == 90 ? 10 : angle == 45 ? 7.0710678 : -7.0710678;
        double ofy = angle == 0 ? 10 : angle == 90 ? 0 : -7.0710678;
        QPointF points[4] = {
            QPointF( ofx + x1*24+12, ofy + y1*24+12),
            QPointF(-ofx + x1*24+12,-ofy + y1*24+12),
            QPointF(-ofx + x2*24+12,-ofy + y2*24+12),
            QPointF( ofx + x2*24+12, ofy + y2*24+12)
        };
        painter->drawPolygon(points, 4);
    }
}
