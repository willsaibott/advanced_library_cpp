#pragma once
#include <QObject>
#include <QtTest/QtTest>

class TestLockable : public QObject
{
  Q_OBJECT
public:
  explicit TestLockable(QObject *parent = nullptr);

signals:

private slots:

  void concurrency_using_std_locks();

  void concurrency_using_std_shared_locks();

  void concurrency_using_std_scoped_locks();

  void test_copy();

  void test_move();

  void test_copy_constructible();

  void test_move_constructible();

  void concurrency_using_qt_locks();

  void concurrency_using_qt_shared_locks();
};


