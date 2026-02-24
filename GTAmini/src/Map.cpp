#include "Map.h"
#include <iostream>
#include <fstream>
#include <random>

// Tile构造函数
Tile::Tile(TileType type, const sf::Texture& texture, sf::Vector2f position)
    : mType(type)
    , mPosition(position)
{
    mSprite.setTexture(texture);
    mSprite.setPosition(position);
}

// 渲染瓦片
void Tile::render(sf::RenderWindow& window)
{
    window.draw(mSprite);
}

// 获取瓦片类型
TileType Tile::getType() const
{
    return mType;
}

// 获取瓦片碰撞边界
sf::FloatRect Tile::getBounds() const
{
    return mSprite.getGlobalBounds();
}

// Map构造函数
Map::Map()
    : mSize(0, 0)
{
    // 加载地图资源
    loadResources();
}

// Map析构函数
Map::~Map()
{
}

// 加载地图资源
void Map::loadResources()
{
    // 加载瓦片纹理
    if (!mTextures[TileType::ROAD].loadFromFile("assets/textures/road.png"))
    {
        std::cerr << "Failed to load road texture!" << std::endl;
    }
    
    if (!mTextures[TileType::BUILDING].loadFromFile("assets/textures/building.png"))
    {
        std::cerr << "Failed to load building texture!" << std::endl;
    }
    
    if (!mTextures[TileType::SIDEWALK].loadFromFile("assets/textures/sidewalk.png"))
    {
        std::cerr << "Failed to load sidewalk texture!" << std::endl;
    }
    
    if (!mTextures[TileType::GRASS].loadFromFile("assets/textures/grass.png"))
    {
        std::cerr << "Failed to load grass texture!" << std::endl;
    }
    
    if (!mTextures[TileType::WATER].loadFromFile("assets/textures/water.png"))
    {
        std::cerr << "Failed to load water texture!" << std::endl;
    }
}

// 从文件加载地图
bool Map::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open map file: " << filename << std::endl;
        return false;
    }
    
    // 读取地图尺寸
    file >> mSize.x >> mSize.y;
    
    // 调整地图瓦片数组大小
    mTiles.resize(mSize.y, std::vector<Tile>(mSize.x, Tile(TileType::ROAD, mTextures[TileType::ROAD], sf::Vector2f(0.f, 0.f))));
    
    // 读取地图数据
    for (int y = 0; y < mSize.y; ++y)
    {
        for (int x = 0; x < mSize.x; ++x)
        {
            int tileType;
            file >> tileType;
            
            TileType type = static_cast<TileType>(tileType);
            sf::Vector2f position(x * 32.f, y * 32.f); // 假设瓦片大小为32x32
            
            mTiles[y][x] = Tile(type, mTextures[type], position);
        }
    }
    
    file.close();
    return true;
}

// 生成随机城市地图
void Map::generateRandomCity(int width, int height)
{
    // 设置地图尺寸
    mSize.x = width;
    mSize.y = height;
    
    // 调整地图瓦片数组大小
    mTiles.resize(mSize.y, std::vector<Tile>(mSize.x, Tile(TileType::ROAD, mTextures[TileType::ROAD], sf::Vector2f(0.f, 0.f))));
    
    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 100);
    
    // 生成基础地形（全部为草地）
    for (int y = 0; y < mSize.y; ++y)
    {
        for (int x = 0; x < mSize.x; ++x)
        {
            sf::Vector2f position(x * 32.f, y * 32.f); // 假设瓦片大小为32x32
            mTiles[y][x] = Tile(TileType::GRASS, mTextures[TileType::GRASS], position);
        }
    }
    
    // 生成道路网格
    for (int y = 0; y < mSize.y; ++y)
    {
        for (int x = 0; x < mSize.x; ++x)
        {
            // 每隔5个瓦片生成一条道路
            if (x % 5 == 0 || y % 5 == 0)
            {
                sf::Vector2f position(x * 32.f, y * 32.f);
                mTiles[y][x] = Tile(TileType::ROAD, mTextures[TileType::ROAD], position);
            }
        }
    }
    
    // 在道路旁边生成人行道
    for (int y = 0; y < mSize.y; ++y)
    {
        for (int x = 0; x < mSize.x; ++x)
        {
            if (mTiles[y][x].getType() == TileType::ROAD)
            {
                // 检查相邻的瓦片
                for (int dy = -1; dy <= 1; ++dy)
                {
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        int nx = x + dx;
                        int ny = y + dy;
                        
                        // 确保在地图范围内
                        if (nx >= 0 && nx < mSize.x && ny >= 0 && ny < mSize.y)
                        {
                            // 如果相邻瓦片是草地，有一定概率变成人行道
                            if (mTiles[ny][nx].getType() == TileType::GRASS && distr(gen) < 70)
                            {
                                sf::Vector2f position(nx * 32.f, ny * 32.f);
                                mTiles[ny][nx] = Tile(TileType::SIDEWALK, mTextures[TileType::SIDEWALK], position);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 在人行道旁边生成建筑物
    for (int y = 0; y < mSize.y; ++y)
    {
        for (int x = 0; x < mSize.x; ++x)
        {
            if (mTiles[y][x].getType() == TileType::SIDEWALK)
            {
                // 检查相邻的瓦片
                for (int dy = -1; dy <= 1; ++dy)
                {
                    for (int dx = -1; dx <= 1; ++dx)
                    {
                        int nx = x + dx;
                        int ny = y + dy;
                        
                        // 确保在地图范围内
                        if (nx >= 0 && nx < mSize.x && ny >= 0 && ny < mSize.y)
                        {
                            // 如果相邻瓦片是草地，有一定概率变成建筑物
                            if (mTiles[ny][nx].getType() == TileType::GRASS && distr(gen) < 40)
                            {
                                sf::Vector2f position(nx * 32.f, ny * 32.f);
                                mTiles[ny][nx] = Tile(TileType::BUILDING, mTextures[TileType::BUILDING], position);
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 生成一些水域
    for (int i = 0; i < width * height / 100; ++i)
    {
        int x = distr(gen) % width;
        int y = distr(gen) % height;
        
        // 只在草地上生成水域
        if (mTiles[y][x].getType() == TileType::GRASS)
        {
            sf::Vector2f position(x * 32.f, y * 32.f);
            mTiles[y][x] = Tile(TileType::WATER, mTextures[TileType::WATER], position);
            
            // 扩展水域
            for (int j = 0; j < 10; ++j)
            {
                int nx = x + (distr(gen) % 5) - 2;
                int ny = y + (distr(gen) % 5) - 2;
                
                // 确保在地图范围内
                if (nx >= 0 && nx < width && ny >= 0 && ny < height)
                {
                    // 只在草地上扩展水域
                    if (mTiles[ny][nx].getType() == TileType::GRASS)
                    {
                        sf::Vector2f position(nx * 32.f, ny * 32.f);
                        mTiles[ny][nx] = Tile(TileType::WATER, mTextures[TileType::WATER], position);
                    }
                }
            }
        }
    }
}

// 更新地图
void Map::update(sf::Time deltaTime)
{
    // 目前地图不需要更新逻辑
}

// 渲染地图
void Map::render(sf::RenderWindow& window)
{
    // 获取窗口视图
    sf::View view = window.getView();
    
    // 计算可见区域
    sf::FloatRect visibleArea(
        view.getCenter().x - view.getSize().x / 2.f,
        view.getCenter().y - view.getSize().y / 2.f,
        view.getSize().x,
        view.getSize().y
    );
    
    // 计算可见瓦片范围
    int startX = std::max(0, static_cast<int>(visibleArea.left / 32.f));
    int startY = std::max(0, static_cast<int>(visibleArea.top / 32.f));
    int endX = std::min(mSize.x - 1, static_cast<int>((visibleArea.left + visibleArea.width) / 32.f + 1));
    int endY = std::min(mSize.y - 1, static_cast<int>((visibleArea.top + visibleArea.height) / 32.f + 1));
    
    // 只渲染可见区域内的瓦片
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            mTiles[y][x].render(window);
        }
    }
}

// 获取地图尺寸
sf::Vector2i Map::getSize() const
{
    return mSize;
}

// 获取指定位置的瓦片
Tile* Map::getTileAt(sf::Vector2f position)
{
    // 计算瓦片坐标
    int x = static_cast<int>(position.x / 32.f);
    int y = static_cast<int>(position.y / 32.f);
    
    // 检查坐标是否在地图范围内
    if (x >= 0 && x < mSize.x && y >= 0 && y < mSize.y)
    {
        return &mTiles[y][x];
    }
    
    return nullptr;
}

// 检查位置是否可通行
bool Map::isPassable(sf::Vector2f position) const
{
    // 计算瓦片坐标
    int x = static_cast<int>(position.x / 32.f);
    int y = static_cast<int>(position.y / 32.f);
    
    // 检查坐标是否在地图范围内
    if (x >= 0 && x < mSize.x && y >= 0 && y < mSize.y)
    {
        TileType type = mTiles[y][x].getType();
        
        // 道路、人行道和草地是可通行的
        return type == TileType::ROAD || type == TileType::SIDEWALK || type == TileType::GRASS;
    }
    
    return false;
}