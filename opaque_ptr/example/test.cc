#include "test.hh"
#include <iostream>

struct Test::Implem
{
  Implem() = default;

  char *test = new char[5];
  ~Implem()
  {
    std::cerr << "destructor called\n";
    delete[] test;
  }

  Implem(const Implem&)
    : test{new char[5]}
  {}

  void print()
  {
    std::cerr << "non const called\n";
  }

  void print() const
  {
    std::cerr << "const called\n";
  }
};

Test::Test()
  : pimpl_{trn::make_opaque<Implem>()}
{
  std::cerr << "size: " << sizeof(pimpl_) << '\n';
}

void Test::test()
{
  pimpl_->print();
}

void Test::test() const
{
  pimpl_->print();
}
