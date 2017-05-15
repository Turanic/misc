#pragma once

#include <memory>

namespace trn
{
  namespace details
  {
    template <typename U>
    void default_deleter(U* ptr)
    {
      delete ptr;
    }

    template <typename U>
    auto default_copier(const U* ptr)
    {
      return new U(*ptr);
    }
  } // details

  template <typename Type,
            typename Deleter = void (*)(Type*),
            typename Copier = Type* (*)(const Type*)>
  class opaque_ptr final
  {
  public:
    template <typename T, typename D = Deleter, typename C = Copier>
    explicit opaque_ptr(T* data,
                        D&& deleter = details::default_deleter,
                        C&& copier = details::default_copier)
        : ptr_{data, std::forward<D>(deleter)}
        , cloner_{std::forward<C>(copier)}
    {
    }

    opaque_ptr(const opaque_ptr& other)
        : opaque_ptr{other.cloner_()}
    {
    }

    opaque_ptr(opaque_ptr&&) noexcept = default;

  private:
    std::unique_ptr<Type, Deleter> ptr_;
    Copier cloner_;

    auto clone() const
    {
      return opaque_ptr{cloner_(ptr_.get()), ptr_.get_deleter(), cloner_};
    }

  };

  template <typename T, typename... Args>
  auto make_opaque(Args&&... args)
  {
    return opaque_ptr<T>(new T(std::forward<Args>(args)...));
  }

} // trn
