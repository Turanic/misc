#pragma once

#include "../opaque_ptr.hh"

class Test
{
public:
  Test();

  void test();
  void test() const;

private:
  struct Implem;
  trn::opaque_ptr<Implem> pimpl_;
};
