#include <memory>

namespace advanced {
namespace tools {

/** @test TestMemory in test/test_memory(.h|.cpp) */

/**
 * @brief make_unique implementation for C++11
 */
template<typename T, typename... Args>
inline std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
} // LCOV_EXCL_LINE

}
}
