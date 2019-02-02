#ifndef CERTA_VECTOR_H_
#define CERTA_VECTOR_H_
#include <cstdlib>
#include <cstring>
#include <sstream>

namespace certa {
template <typename _T = double>
class vec {
 public:
  vec() : size_(0) { data_ = (_T*)std::malloc(0 * sizeof(_T)); }
  explicit vec(const unsigned long& n) : size_(n) {
    data_ = (_T*)std::malloc(n * sizeof(_T));
    fill(_T());
  }

  vec(const std::initializer_list<_T>& args) : size_(args.size()) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::memcpy(data_, args.begin(), args.size() * sizeof(_T));
  }

  template <typename... _ARGS>
  explicit vec(const unsigned long& n, const _ARGS&... args) : size_(n) {
    data_ = (_T*)std::malloc(n * sizeof(_T));
    fill(_T());
    constructor<0>(args...);
  }
  vec(const vec<_T>& cpy) : size_(cpy.size_) {
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::memcpy(data_, cpy.data(), size_ * sizeof(_T));
  }

  ~vec() { free(data_); }

  inline vec<_T>& operator=(const std::initializer_list<_T>& args) {
    free(data_);
    size_ = args.size();
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::memcpy(data_, args.begin(), args.size() * sizeof(_T));
    return *this;
  }

  inline vec<_T>& operator=(const vec<_T>& cpy) {
    free(data_);
    size_ = cpy.size();
    data_ = (_T*)std::malloc(size_ * sizeof(_T));
    std::memcpy(data_, cpy.begin(), cpy.size() * sizeof(_T));
    return *this;
  }

  inline _T& operator[](const unsigned long& i) { return data_[i]; }
  inline const _T& operator[](const unsigned long& i) const { return data_[i]; }

  inline _T& at(const unsigned long& i) { return data_[i]; }
  inline const _T& at(const unsigned long& i) const { return data_[i]; }

  inline _T& front() { return data_[0]; }
  inline const _T& front() const { return data_[0]; }

  inline _T& back() { return data_[size_ - 1]; }
  inline const _T& back() const { return data_[size_ - 1]; }

  inline _T* begin() noexcept { return data_; }
  inline const _T* begin() const noexcept { return data_; }
  inline const _T* cbegin() const noexcept { return data_; }
  inline _T* end() noexcept { return data_ + size_; }
  inline const _T* end() const noexcept { return data_ + size_; }
  inline const _T* cend() const noexcept { return data_ + size_; }

  inline _T* data() noexcept { return data_; }
  inline const _T* data() const noexcept { return data_; }

  inline std::string str() const {
    std::stringstream ss;
    ss << '<';
    for (unsigned long i = 0; i < size_ - 1; ++i) {
      ss << data_[i] << ',';
    }
    ss << data_[size_ - 1] << '>';
    return ss.str();
  }

  inline std::string raw() const {
    std::string str = "<";
    for (unsigned long i = 0; i < size_ - 1; ++i) {
      str += std::to_string(data_[i]) + ' ';
    }
    str += std::to_string(data_[size_ - 1]) + '>';
    return str;
  }

  inline unsigned long size() const noexcept { return size_; }

  inline void resize(const unsigned long& n) {
    _T* new_data_ = (_T*)realloc(data_, n * sizeof(_T));
    if (new_data_) {
      data_ = new_data_;
    } else {
      free(data_);
    }
    unsigned long old_size = size_;
    size_ = n;
    if (old_size < n) {
      fill(_T(), old_size);
    }
  }
  inline void resize(const unsigned long& n, const _T& v) {
    _T* new_data_ = (_T*)realloc(data_, n * sizeof(_T));
    if (new_data_) {
      data_ = new_data_;
    } else {
      free(data_);
    }
    unsigned long old_size = size_;
    size_ = n;
    if (old_size < n) {
      fill(v, old_size);
    }
  }

  inline void fill(const _T& v) {
    for (unsigned long i = 0; i < size_; ++i) {
      data_[i] = v;
    }
  }
  inline void fill(_T (*func)()) {
    for (unsigned long i = 0; i < size_; ++i) {
      data_[i] = func();
    }
  }
  inline void fill(_T (*func)(const unsigned long&)) {
    for (unsigned long i = 0; i < size_; ++i) {
      data_[i] = func(i);
    }
  }
  inline void fill(const _T& v, const unsigned long& start) {
    for (unsigned long i = start; i < size_; ++i) {
      data_[i] = v;
    }
  }
  inline void fill(const _T& v, const unsigned long& start,
                   const unsigned long& end) {
    for (unsigned long i = start; i < end; ++i) {
      data_[i] = v;
    }
  }
  inline void clear() { fill(_T()); }

 private:
  template <std::size_t _N>
  typename std::enable_if<_N != 0, void>::type constructor(const _T& v) {
    data_[_N] = v;
  }
  template <std::size_t _N>
  typename std::enable_if<_N == 0, void>::type constructor(const _T& v) {
    fill(v);
  }
  template <std::size_t _N, typename... _ARGS>
  void constructor(const _T& v, const _ARGS&... args) {
    data_[_N] = v;
    constructor<_N + 1>(args...);
  }

  unsigned long size_;
  _T* data_;
};

template <typename _T>
inline std::ostream& operator<<(std::ostream& out, const vec<_T>& rhs) {
  out << rhs.str();
  return out;
}

template <typename _T, typename _U>
inline bool operator==(const vec<_T>& lhs, const vec<_U>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename _T, typename _U>
inline bool operator!=(const vec<_T>& lhs, const vec<_U>& rhs) {
  return !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename _T, typename _U>
inline bool operator<(const vec<_T>& lhs, const vec<_U>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

template <typename _T, typename _U>
inline bool operator>(const vec<_T>& lhs, const vec<_U>& rhs) {
  return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(),
                                      lhs.end());
}

template <typename _T, typename _U>
inline bool operator<=(const vec<_T>& lhs, const vec<_U>& rhs) {
  return !std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(),
                                       lhs.end());
}

template <typename _T, typename _U>
inline bool operator>=(const vec<_T>& lhs, const vec<_U>& rhs) {
  return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                       rhs.end());
}

template <typename _T, typename _U>
inline vec<_T> operator+(const vec<_T>& lhs, const _U& rhs) {
  vec<_T> res(lhs.size());
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] + rhs;
  }
  return res;
}
template <typename _T, typename _U>
inline vec<_T> operator+(const vec<_T>& lhs, const vec<_U>& rhs) {
  vec<_T> res(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] + rhs[i];
  }
  return res;
}

template <typename _T, typename _U>
inline vec<_T> operator-(const vec<_T>& lhs, const _U& rhs) {
  vec<_T> res(lhs.size());
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] - rhs;
  }
  return res;
}
template <typename _T, typename _U>
inline vec<_T> operator-(const vec<_T>& lhs, const vec<_U>& rhs) {
  vec<_T> res(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] - rhs[i];
  }
  return res;
}

template <typename _T, typename _U>
inline vec<_T> operator*(const vec<_T>& lhs, const _U& rhs) {
  vec<_T> res(lhs.size());
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] * rhs;
  }
  return res;
}
template <typename _T, typename _U>
inline _T operator*(const vec<_T>& lhs, const vec<_U>& rhs) {
  _T sum = _T();
  for (unsigned long i = 0; i < std::min(lhs.size(), rhs.size()); ++i) {
    sum += lhs[i] * rhs[i];
  }
  return sum;
}

template <typename _T, typename _U>
inline vec<_T> operator/(const vec<_T>& lhs, const _U& rhs) {
  vec<_T> res(lhs.size());
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] / rhs;
  }
  return res;
}
template <typename _T, typename _U>
inline vec<_T> operator/(const vec<_T>& lhs, const vec<_U>& rhs) {
  vec<_T> res(std::min(lhs.size(), rhs.size()));
  for (unsigned long i = 0; i < res.size(); ++i) {
    res[i] = lhs[i] / rhs[i];
  }
  return res;
}

}  // namespace certa

#endif  // CERTA_VECTOR_H_
