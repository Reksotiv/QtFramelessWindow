/*
###############################################################################
#                                                                             #
#                           The MIT License                                   #
# Copyright (C) 2020 by Reksotiv (hellfire0h@gmail.com)                       #
#                https://github.com/Reksotiv                                  #
# Sources code: https://github.com/Reksotiv/QtFramelessWindow                 #
#                                                                             #
###############################################################################
*/

#ifndef CUSTOMSHADOWEFFECT_H
#define CUSTOMSHADOWEFFECT_H

#include <QGraphicsDropShadowEffect>


class CustomShadowEffect : public QGraphicsDropShadowEffect
{
    Q_OBJECT
public:
    explicit CustomShadowEffect(QObject *parent = 0);

protected:
    QRectF boundingRectFor(const QRectF& rect) const override;
};

#endif // CUSTOMSHADOWEFFECT_H
