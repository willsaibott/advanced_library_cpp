#pragma once

#include <QObject>
#include <QTest>

class TestReadCsv : public QObject
{
  Q_OBJECT
public:
  explicit TestReadCsv(QObject *parent = nullptr);

signals:

private slots:
  void test_read_csv_file();
};
