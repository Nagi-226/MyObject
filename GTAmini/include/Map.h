#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

// 地图瓦片类型枚举
enum class TileType {
    ROAD,
    BUILDING,
    SIDEWALK,
    GRASS,
    WATER
};

// 地图瓦片类
class Tile {
public:
    Tile(TileType type, const sf::Texture& texture, sf::Vector2f position);
    
    void render(sf::RenderWindow& window);
    
    TileType getType() const;
    
    sf::FloatRect getBounds() const;

private:
    TileType mType;
    sf::Sprite mSprite;
    sf::Vector2f mPosition;
};

// 地图类
class Map {
public:
    // 构造函数和析构函数
    Map();
    ~Map();

    // 加载地图
    bool loadFromFile(const std::string& filename);
    
    // 生成随机城市地图
    void generateRandomCity(int width, int height);
    
    // 更新地图
    void update(sf::Time deltaTime);
    
    // 渲染地图
    void render(sf::RenderWindow& window);

    // 获取地图尺寸
    sf::Vector2i getSize() const;
    
    // 获取瓦片在指定位置
    Tile* getTileAt(sf::Vector2f position);
    
    // 检查位置是否可通行
    bool isPassable(sf::Vector2f position) const;

private:
    // 加载地图资源
    void loadResources();

    // 地图尺寸
    sf::Vector2i mSize;
    
    // 地图瓦片
    std::vector<std::vector<Tile>> mTiles;
    
    // 地图纹理
    std::unordered_map<TileType, sf::Texture> mTextures;
};

#endif // MAP_H