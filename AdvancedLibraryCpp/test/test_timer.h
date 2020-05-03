#pragma once

#include <QObject>

class TestTimer : public QObject
{
  Q_OBJECT
public:
  explicit TestTimer(QObject *parent = nullptr);

signals:

public slots:
};

