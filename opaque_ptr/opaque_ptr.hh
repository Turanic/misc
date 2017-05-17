#pragma once

#include <experimental/propagate_const>
#include <memory>

namespace trn
{
  namespace details
  {
    template <typename U>
    void default_deleter(U* ptr)
    {
      static_assert(sizeof(ptr) > 0, "can't delete pointer to incomplete type");
      delete ptr;
    }

    template <typename U>
    auto default_copier(const U* ptr)
    {
      static_assert(sizeof(ptr) > 0, "can't copy pointer to incomplete type");
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
        : ptr_{std::unique_ptr<T, D>{data, std::forward<D>(deleter)}}
        , cloner_{std::forward<C>(copier)}
    {
    }

    template <typename T, typename D = Deleter, typename C = Copier>
    explicit opaque_ptr(D&& deleter = details::default_deleter,
                        C&& copier = details::default_copier)
        : opaque_ptr{nullptr, std::forward<D>(deleter), std::forward<C>(copier)}
    {
    }

    ~opaque_ptr() noexcept = default;

    opaque_ptr(const opaque_ptr& other)
        : opaque_ptr{other.cloner_(other.get()),
                     other.unique_get().get_deleter(),
                     other.cloner_}
    {
    }

    opaque_ptr(opaque_ptr&&) noexcept = default;

    opaque_ptr& operator=(const opaque_ptr& other)
    {
      ptr_ = {other.cloner_(other.get()), other.unique_get().get_deleter()};
      cloner_ = other.cloner_;

      return *this;
    }

    opaque_ptr& operator=(opaque_ptr&&) noexcept = default;

    decltype(auto) get() const
    {
      return ptr_.get();
    }

    decltype(auto) get()
    {
      return ptr_.get();
    }

    decltype(auto) operator-> () const
    {
      return ptr_.operator->();
    }

    decltype(auto) operator-> ()
    {
      return ptr_.operator->();
    }

    decltype(auto) operator*() const
    {
      return ptr_.operator*();
    }

    decltype(auto) operator*()
    {
      return ptr_.operator*();
    }

  private:
    decltype(auto) unique_get() const noexcept
    {
      return std::experimental::get_underlying(ptr_);
    }

    template <typename T>
    using const_ptr_t = std::experimental::propagate_const<T>;
    using pointer_t = const_ptr_t<std::unique_ptr<Type, Deleter>>;

    pointer_t ptr_;
    Copier cloner_;
  };

  template <typename T, typename... Args>
  auto make_opaque(Args&&... args)
  {
    return opaque_ptr<T>(new T(std::forward<Args>(args)...));
  }

} // trn
