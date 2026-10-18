#include <memory>
#include <vector>

namespace rogueutil {


    class MapBase
    {
    public:
        MapBase() {}
        ~MapBase() {}
    private:
    };

    struct Coord
    {
        int x;
        int y;
    };

    template<typename T>
    struct CellRef
    {
        Coord coord;
        T& value;
    };

    template<typename T>
    class Map
    {
    public:
        class cell_view;

        explicit Map(std::size_t width, std::size_t height, T default_value)
            : m_width(width)
            , m_height(height)
            , m_cells(width * height, default_value)
            , m_default_value(default_value)
        {
        }

        Map() = default;
        Map(const Map&) = default;
        Map(Map&&) = default;
        Map& operator=(const Map&) = default;
        Map& operator=(Map&&) = default;
        auto operator<=>(const Map&) const = default;
        virtual ~Map() = default;

        int width() const { return m_width; }
        int height() const { return m_height; }

        T& at(std::size_t x, std::size_t y)
        {
            return m_cells[y * m_width + x];
        }

        const T& at(std::size_t x, std::size_t y) const
        {
            return m_cells[y * m_width + x];
        }

        /// @brief Returns the list of cells that can then be iterated over.
        /// @return A list of cells and there co-ordinates.
        /// @code{C++}
        ///     for (auto&& [coord, value] : map.cells())
        ///     {
        ///         const auto [x, y] = coord;
        ///         value += 1;
        ///     }
        /// @endcode
        cell_view cells()
        {
            return cell_view(*this);
        }

        void clear() { m_cells.clear(); m_cells.resize(m_width * m_height, m_default_value); }

    private:
        int m_width;
        int m_height;
        std::vector<T> m_cells;
        T m_default_value;
        int m_world_x{ 0 };
        int m_world_y{ 0 };

        friend class cell_view;
    };


    template<typename T>
    class Map<T>::cell_view
    {
    public:
        explicit cell_view(Map& m) : m_map(m) {}

        class iterator
        {
        public:
            using iterator_concept  = std::forward_iterator_tag;
            using iterator_category = std::forward_iterator_tag;
            using value_type        = CellRef<T>;
            using difference_type   = std::ptrdiff_t;

            iterator() = default;

            iterator(Map* map, std::size_t index)
                : m_index(index)
                , m_map(map)
            {
            }

            value_type operator*()
            {
                return CellRef<T>{
                        Coord{ static_cast<int>(m_index % m_map->m_width), static_cast<int>(m_index / m_map->m_width) },
                        m_map->m_cells[m_index]
                    };
            }

            iterator& operator++()
            {
                ++m_index;
                return *this;
            }

            iterator operator++(int)
            {
                auto tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const iterator& lhs, const iterator& rhs)
            {
                return lhs.m_index == rhs.m_index;
            }

            friend bool operator!=(const iterator& lhs, const iterator& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            std::size_t m_index{ 0 };
            Map* m_map{ nullptr };
        };

        iterator begin() { return iterator(&m_map, 0); }
        iterator end() { return iterator(&m_map, m_map.m_cells.size()); }

    private:
        Map& m_map;
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

        DungeonMap() = default;
        DungeonMap(const DungeonMap&) = default;
        DungeonMap(DungeonMap&&) = default;
        DungeonMap& operator=(const DungeonMap&) = default;
        DungeonMap& operator=(DungeonMap&&) = default;
        virtual ~DungeonMap() = default;

        void setTileType(int x, int y, TileType t) { auto& value = at(x, y); value = (value & ~TILE_TYPE_MASK) | t; }
        bool isVisible(int x, int y) const { return (at(x, y) & VISIBLE_BIT) == VISIBLE_BIT; }
        bool isSeen(int x, int y) const { return (at(x, y) & SEEN_BIT) == SEEN_BIT; }
        bool isVisited(int x, int y) const { return (at(x, y) & VISITED_BIT) == VISITED_BIT; }
        void markVisible(int x, int y) { auto& cell = at(x, y); cell |= VISIBLE_BIT; }
        void markSeen(int x, int y) { auto& cell = at(x, y); cell |= SEEN_BIT; }
        void markVisited(int x, int y) { auto& cell = at(x, y); cell |= VISITED_BIT; }
        void clearCell(int x, int y) {  at(x, y) = 0; }
    private:
    };

    static_assert(DungeonMap::TileType::TILE_TYPE_MAX <= DungeonMap::TILE_TYPE_MASK, "Maximum number of tile types exceeded.");

    template<typename T>
    using MapPtr = std::shared_ptr<Map<T>>;

    using DungeonMapPtr = std::shared_ptr<Map<unsigned>>;
}
