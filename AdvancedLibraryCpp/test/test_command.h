#pragma once

#include <QObject>

class TestCommand : public QObject
{
  Q_OBJECT
public:
  explicit TestCommand(QObject *parent = nullptr);

signals:

public slots:
};

