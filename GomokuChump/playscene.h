#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPainter>
#include "playingboard.h"

class playScene : public QGraphicsScene
{
    Q_OBJECT
private:
    playingBoard* board_;
public:
    playScene(playingBoard* board);
    void replaceBoard(playingBoard* board);
protected:
    void drawBackground(QPainter * painter, const QRectF & rect);

};

#endif // PLAYSCENE_H
