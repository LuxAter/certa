#ifndef CERTA_VECTOR_H_
#define CERTA_VECTOR_H_
#include <cstdlib>
#include <sstream>

namespace certa {
template <typename _T>
class vec {
 public:
  vec() : size_(0) { data_ = (_T*)std::malloc(0 * sizeof(_T)); }
  explicit vec(const unsigned long& n) : size_(n) {
    data_ = (_T*)std::malloc(n * sizeof(_T));
    memset(data_, _T(), size_ * sizeof(_T));
  }
  explicit vec(const unsigned long& n, const _T& v) : size_(n) {
    data_ = (_T*)std::malloc(n * sizeof(_T));
    memset(data_, v, size_ * sizeof(_T));
  }

  inline _T& operator[](const unsigned long& i) { return data_[i]; }
  inline const _T& operator[](const unsigned long& i) const { return data_[i]; }

  inline _T& at(const unsigned long& i) { return data_[i]; }
  inline const _T& at(const unsigned long& i) const { return data_[i]; }

  inline _T& front() { return data_[0]; }
  inline const _T& front() const { return data_[0]; }

  inline _T& back() { return data_[size_ - 1]; }
  inline const _T& back() const { return data_[size_ - 1]; }

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

  inline unsigned long size() const noexcept { return size_; }

  inline void resize(const unsigned long& n) {
    _T* new_data_ = (_T*)realloc(data_, n * sizeof(_T));
    if (new_data_) {
      data_ = new_data_;
    } else {
      free(data_);
    }
    if (size_ < n) {
      memset(data_ + size_, _T(), (n - size_) * sizeof(_T));
    }
    size_ = n;
  }
  inline void resize(const unsigned long& n, const _T& v) {
    _T* new_data_ = (_T*)realloc(data_, n * sizeof(_T));
    if (new_data_) {
      data_ = new_data_;
    } else {
      free(data_);
    }
    if (size_ < n) {
      memset(data_ + size_, v, (n - size_) * sizeof(_T));
    }
    size_ = n;
  }

  inline void fill(const _T& v) { memset(data_, v, size_ * sizeof(_T)); }
  inline void clear() { memset(data_, _T(), size_ * sizeof(_T)); }

 private:
  unsigned long size_;
  _T* data_;
};
}  // namespace certa

#endif  // CERTA_VECTOR_H_
