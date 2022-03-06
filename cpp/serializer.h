#include <iostream>
#include <vector>

typedef unsigned char byte_t;
typedef std::vector<byte_t> buffer;

class serializer {
 public:
  serializer(bool verbose = false);
  serializer(buffer message, bool verbose = false);
  ~serializer();

  void print_container();
  buffer dump_container();
  void clear();

  template <typename T>
  void push_back(const T& obj) {
    serialize(obj);
  }

  template <typename T>
  T pop_front(const T& obj) {
    T ret = (T)0;
    try {
      ret = deserialize<T>();
      strip_buffer(obj);
    } catch (const std::exception& e) {
      std::cout << e.what() << '\n';
      //  throw e; // copy-initializes a new exception object of type
      //  std::exception
      throw;  // rethrows the exception object of type std::length_error
    }
    return ret;
  }

 private:
  buffer _message;
  bool _verbose = false;

  template <typename T>
  std::size_t object_size(T const& obj) {
    return sizeof(obj);
  };

  template <typename T>
  void serialize(const T& obj) {
    size_t start_size_buffer = _message.size();
    size_t size_new_obj = object_size(obj);
    buffer buf(start_size_buffer + size_new_obj);

    buf.insert(buf.begin(), _message.data(), _message.data() + start_size_buffer);

    byte_t const* obj_begin = reinterpret_cast<byte_t const*>(&obj);
    copy(obj_begin, obj_begin + size_new_obj, buf.begin() + start_size_buffer);

    buf.resize(start_size_buffer + size_new_obj);
    _message = buf;

    if (_verbose) {
      print_container();
    }
  };

  template <typename T>
  T deserialize() {
    T obj;
    if (_message.size() < object_size(obj)) {
      std::cout << "Buffer too small: length=" << _message.size()
                << " -- obj-size=" << object_size(obj) << std::endl;
      throw std::length_error("buffer too small for object");
    }
    return *reinterpret_cast<const T*>(&_message[0]);
  }

  template <typename T>
  void strip_buffer(const T& obj) {
    if (_message.size() >= object_size(obj)) {
      _message.erase(_message.begin(), _message.begin() + object_size(obj));
    } else {
      std::cout << "Buffer too small: length=" << _message.size()
                << "obj-size=" << object_size(obj) << std::endl;
    }
  }
};