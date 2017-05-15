#pragma once

#include "../opaque_ptr.hh"

class Test
{
public:
  Test();

private:
  struct Implem;
  trn::opaque_ptr<Implem> pimpl_;
};
