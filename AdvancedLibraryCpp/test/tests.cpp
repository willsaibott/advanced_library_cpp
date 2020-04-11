#include "testlockable.h"
#include "testenum.h"

int
main(int argc, char** argv) {
   int status = 0;
   {
      TestLockable tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   {
      TestEnum tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   return status;
}
