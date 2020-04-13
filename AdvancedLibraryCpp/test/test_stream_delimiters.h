#pragma once

#include <QObject>
#include <QTest>

class TestStreamDelimiters : public QObject
{
  Q_OBJECT
public:
  explicit TestStreamDelimiters(QObject *parent = nullptr);

signals:

private slots:

  void test_string_stream_words_splitter();

  void test_string_stream_lowercase_only();

  void test_string_stream_uppercase_only();

  void test_string_stream_digit_only_only();
};

