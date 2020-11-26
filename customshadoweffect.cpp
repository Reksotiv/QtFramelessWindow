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

#include "customshadoweffect.h"

CustomShadowEffect::CustomShadowEffect(QObject *parent) : QGraphicsDropShadowEffect(parent)
{
}

QRectF CustomShadowEffect::boundingRectFor(const QRectF& rect) const
{
#ifdef Q_OS_WIN
    qreal delta = 0;
#else
    qreal delta = blurRadius() + offset();
#endif
    return rect.united(rect.adjusted(-delta, -delta, delta, delta));
}
