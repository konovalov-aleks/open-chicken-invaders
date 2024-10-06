#include <cstddef>
#include <concepts>
#include <functional>
#include <string_view>

namespace oci {

struct TransparentStrHash {
    using is_transparent = void;
    template <typename StrT>
    requires std::constructible_from<std::string_view, StrT>
    std::size_t operator() (const StrT& s) const noexcept {
        return std::hash<std::string_view>()(s);
    }
};

} // namespace oci
