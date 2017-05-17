#include "test.hh"

int main()
{
  Test test{};
  const Test test2{test};

  test.test();
  test2.test();

  return 0;
}
