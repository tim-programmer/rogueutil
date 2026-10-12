#include <iterator>
#include <tuple>
#include <vector>

namespace rogueutil {

    class MapBase
    {
    public:
        MapBase() {}
        ~MapBase() {}
    private:
    };

    template<typename T>
    class Map
    {
    public:
        struct iterator_type
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using element_type = value_type;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = std::ptrdiff_t;

            iterator_type() = default;
            //explicit iterator_type(T vv) : v(vv) {}
            explicit iterator_type(Map& mm) : m(mm) {}
            int x{ 0 };
            int y{ 0 };
            Map<T>& m;
            //std::vector<T>::iterator v;
            iterator_type& operator++() {
                if(++x >= m.width_) {
                    x = 0;
                    ++y;
                }
                return *this;
            }
            iterator_type operator++(int) {
                iterator_type tmp = *this;
                ++(*this);
                return tmp;
            }
            bool operator==(const iterator_type& rhs) {
                return x == rhs.x && y == rhs.y;
            }
            bool operator!=(const iterator_type& rhs) {
                return !(this == rhs);
            }
            std::tuple<int, int, T> operator*() const {
                return std::make_tuple(x, y, m.get(x, y));
            }
        };

        Map(int width, int height, T default_value = T(0))
            : width_(width)
            , height_(height)
            , default_value_(default_value)
        {
            cells_.resize(width_ * height_, default_value_);
        }
        virtual ~Map() {}
        void clear() { cells_.clear(); cells_.resize(width_ * height_, default_value_); }
        const T get(int x, int y) const { return cells_[x + y * width_]; }
        void set(int x, int y, T value) { cells_[x + y * width_] = value; }

        static_assert(std::forward_iterator<iterator_type>);
        iterator_type begin() { return iterator_type{ this }; }
        iterator_type end() { return iterator_type{ }; }
    private:
        int world_x_{ 0 };
        int world_y_{ 0 };
        int width_{ 0 };
        int height_{ 0 };
        T default_value_{ 0 };
        std::vector<T> cells_;
    };

    class DungeonMap : public Map<unsigned>
    {
    public:
        static const unsigned TILE_TYPE_MASK = ((1u << 16) - 1);

        enum TileType {
            TILE_VOID,
            TILE_FLOOR,
            TILE_WALL,
            TILE_TYPE_MAX,
        };
        enum {
            VISIBLE_BIT     = 1u << 16,
            SEEN_BIT        = 1u << 17,
            VISITED_BIT     = 1u << 18,
        };
        DungeonMap(int width, int height) : Map<unsigned>(width, height, 0) {}
        virtual ~DungeonMap() {}
        void setTileType(int x, int y, TileType t) { unsigned value = get(x, y) & ~TILE_TYPE_MASK; value |= t; set(x, y, value); }
        bool isVisible(int x, int y) const { return (get(x, y) & VISIBLE_BIT) == VISIBLE_BIT; }
        bool isSeen(int x, int y) const { return (get(x, y) & SEEN_BIT) == SEEN_BIT; }
        bool isVisited(int x, int y) const { return (get(x, y) & VISITED_BIT) == VISITED_BIT; }
        void markVisible(int x, int y) { set(x, y, get(x, y) | VISIBLE_BIT); }
        void markSeen(int x, int y) { set(x, y, get(x, y) | SEEN_BIT); }
        void markVisited(int x, int y) { set(x, y, get(x, y) | VISITED_BIT); }
        void clearCell(int x, int y) { set(x, y, 0); }
    private:
    };

    static_assert(DungeonMap::TileType::TILE_TYPE_MAX <= DungeonMap::TILE_TYPE_MASK, "Maximum number of tile types exceeded.");

}
