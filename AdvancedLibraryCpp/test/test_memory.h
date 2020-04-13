#pragma once

#include <QObject>
#include <QTest>

class TestMemory : public QObject
{
  Q_OBJECT
public:
  explicit TestMemory(QObject *parent = nullptr);

signals:

private slots:
  void test_make_unique_for_cpp_11();
};


class base_class {
  int a;
public:
  base_class(int a) : a{ a } {}
  virtual ~base_class(){}

  int getA() const {
    return a;
  }

  virtual int getB() const = 0;
};

class derived : public base_class {
  int b;
public:
  derived(int a, int b) : base_class{ a }, b{ b } {}
  virtual ~derived() {}

  virtual int getB() const override {
    return b;
  }
};
