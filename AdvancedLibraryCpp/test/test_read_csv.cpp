#include "test_read_csv.h"
#include <limits>
#include <stdio.h>
#include "../tools/read_csv.h"

using namespace advanced;
using namespace advanced::tools;

TestReadCsv::
TestReadCsv(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestReadCsv");
}

void TestReadCsv::
test_read_csv_file() {
    Q_INIT_RESOURCE(tests);

    using namespace std;
    using namespace advanced;
    using namespace tools;

    std::vector<CSV_FIELD_TYPE> file_descriptor = {
        CSV_FIELD_TYPE::DOUBLE,
        CSV_FIELD_TYPE::DOUBLE,
        CSV_FIELD_TYPE::DOUBLE,
        CSV_FIELD_TYPE::DOUBLE,
        CSV_FIELD_TYPE::STR};

    QFile file(":/resources/testdata/iris.data");
    if (file.exists()) {
      file.open(QIODevice::ReadOnly);
      QDir dir(QDir::currentPath()+"/testdata");
      if (!dir.exists()){
        dir.mkdir( QDir::currentPath()+"/testdata");
      }
      QFile testfile(QDir::currentPath()+"/testdata/iris.data");
      testfile.open(QIODevice::WriteOnly);
      testfile.write(file.readAll());
      testfile.close();
      csv_reader_t f(file_descriptor);
      auto filename{ (QDir::currentPath()+"/testdata/iris.data").toStdString() };
      csv_file_t r = f.read_csv_file(filename);
      QVERIFY((r->list.front()[0]->get_double() > 5.0) &&
              (r->list.front()[0]->get_double() < 5.2) &&
              (r->list.front()[4]->string() == "Iris-setosa"));
    }
}

