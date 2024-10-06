namespace oci {

struct TransparentEqual {
    using is_transparent = void;
    template <typename T1, typename T2>
    bool operator() (const T1& a, const T2& b) const noexcept {
        return a == b;
    }
};

} // namespace oci
