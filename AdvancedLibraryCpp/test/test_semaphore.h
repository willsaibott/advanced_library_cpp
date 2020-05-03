#pragma once

#include <QObject>

class TestSemaphore : public QObject
{
  Q_OBJECT
public:
  explicit TestSemaphore(QObject *parent = nullptr);

signals:

public slots:
};

