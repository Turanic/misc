#include "test.hh"
#include <iostream>

struct Test::Implem
{
  char *test = new char[5];
  ~Implem()
  {
    std::cerr << "destructor called\n";
    delete[] test;
  }
};

Test::Test()
  : pimpl_{trn::make_opaque<Implem>()}
{
}
