#include "typedef.h"

/*
* -----------------------------
* 1. 图像结构体 (封装 stb_image)
* -----------------------------
*/

// 初始化预定义静态颜色常量
const Image::Color Image::Color::BLACK    =   { 0,   0,   0};
const Image::Color Image::Color::RED      =   {255,  0,   0};
const Image::Color Image::Color::GREEN    =   { 0,  255,  0};
const Image::Color Image::Color::BLUE     =   { 0,   0,  255};
const Image::Color Image::Color::WHITE    =   {255, 255, 255};
const Image::Color Image::Color::YELLOW   =   {255, 255,  0};

Image::Color Image::Color::randColor() // 生成随机颜色
{
    static thread_local std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> dis(0, 255);
    return {
        static_cast<unsigned char>(dis(gen)),
        static_cast<unsigned char>(dis(gen)),
        static_cast<unsigned char>(dis(gen))};
};

Image::Image(int w, int h, Channel c)
        : width(w), height(h), channel(c)
{
    image_buffer = new stbi_uc[w * h * c]; // 分配图像内存并初始化为 0
}

void Image::setColor(const Pixel& pixel, const Color& color)
{
    // 设置指定像素的颜色
    stbi_uc* pixel_buffer = image_buffer + (pixel.y * width + pixel.x) * channel;
    pixel_buffer[0] = color.R;
    pixel_buffer[1] = color.G;
    pixel_buffer[2] = color.B;
    pixel_buffer[3] = 255; // 透明度
}

void Image::flipVertical() {
    const size_t bytesPerRow = width * static_cast<size_t>(channel);
    stbi_uc* tempRow = new stbi_uc[bytesPerRow];
    
    for (uint32_t i = 0; i < height / 2; ++i) {
        const size_t topOffset = i * bytesPerRow;
        const size_t bottomOffset = (height - 1 - i) * bytesPerRow;
        
        // 交换上下行数据
        memcpy(tempRow, image_buffer + topOffset, bytesPerRow);
        memcpy(image_buffer + topOffset, image_buffer + bottomOffset, bytesPerRow);
        memcpy(image_buffer + bottomOffset, tempRow, bytesPerRow);
    }
    
    delete[] tempRow;
}

void Image::load(const char* filename) // 加载图像文件
{
    int x, y, c;
    image_buffer = stbi_load(filename, &x, &y, &c, 0);
    if (image_buffer)
    {
        width = x;
        height = y;
        channel = Image::Channel(c);
        spdlog::info("Loaded image from {}, width : {}, height : {}, Channel : {}", filename, width, height, (int) channel);
    }
    else
    {
        spdlog::error("Failed to load image from {}", filename);
        exit(1);
    }
}

void Image::save(const char* filename)
{
    bool is_saved = stbi_write_png(filename, width, height, channel, image_buffer, width * channel); // 写入图像文件
    image_buffer = nullptr; // stbi_write_png() 会自动释放图像数据
    if (is_saved)
        spdlog::info("Image saved to {}", filename);
    else
        spdlog::error("Failed to save image to {}", filename);
}

/*
* ---------------------------------
* 2. 3D 模型类 (封装 tinyobjloader)
* ---------------------------------
*/

void Model::setLoadConfig(bool triangulate, bool vertex_color, std::string triangulation_method, std::string mtl_search_path)
{
    readerConfig.triangulate            =   triangulate;
    readerConfig.vertex_color           =   vertex_color;
    readerConfig.triangulation_method   =   triangulation_method;
    readerConfig.mtl_search_path        =   mtl_search_path;
}

void Model::loadFrom(const tinyobj::attrib_t& tinyattrib, const std::vector<tinyobj::shape_t>& tinyshapes) // 从 tinyobjloader 属性加载自定义数据结构
{
    for (size_t i = 0; i < tinyattrib.vertices.size(); i += 3)
    {
        attrib.vertices.push_back({
            tinyattrib.vertices[i + 0], // x
            tinyattrib.vertices[i + 1], // y
            tinyattrib.vertices[i + 2] // z
        });
    }
    for (size_t i = 0; i < tinyattrib.normals.size(); i += 3)
    {
        attrib.normals.push_back({
            tinyattrib.normals[i + 0], // nx
            tinyattrib.normals[i + 1], // ny
            tinyattrib.normals[i + 2] // nz
        });
    }
    for (size_t i = 0; i < tinyattrib.texcoords.size(); i += 2)
    {
        attrib.texcoords.push_back({
            tinyattrib.texcoords[i + 0], // u
            tinyattrib.texcoords[i + 1] // v
        });
    }

    size_t index_offset = 0, s = 0;
    for (size_t f = 0; f < tinyshapes[s].mesh.num_face_vertices.size(); f++) // 遍历每个面
    { 
        size_t fv = 3; // 三角面的顶点数
        triface_t face;
        for (size_t v = 0; v < fv; v++) // 遍历每个顶点
        {
            tinyobj::index_t idx = tinyshapes[s].mesh.indices[index_offset + v];
            face.vertex[v] = {idx.vertex_index, idx.normal_index, idx.texcoord_index};
        }
        trimesh.trifaces.push_back(face);
        trimesh.material_ids.push_back(tinyshapes[s].mesh.material_ids[f]);
        index_offset += fv;
    }
}

void Model::loadFrom(const std::string& filename) // 从文件加载模型
{
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename, readerConfig)) 
    {
        // 加载失败，打印错误信息
        if (!reader.Error().empty())
            spdlog::error("TinyObjReader: {}", reader.Error());
        
        exit(1); // 退出程序
    }

    if (!reader.Warning().empty()) 
    {
        // 打印警告信息
        spdlog::warn("TinyObjReader: {}", reader.Warning());
    }

    auto& tinyattrib = reader.GetAttrib();
    auto& tinyshapes = reader.GetShapes();
    loadFrom(tinyattrib, tinyshapes); // 加载自定义数据结构
    
    materials = reader.GetMaterials(); // 获取材质列表

    // 打印加载信息
    spdlog::info(
        "Loaded model from {}: "
        "{} vertices, {} faces, {} materials",
        filename,
        attrib.vertices.size(),
        trimesh.trifaces.size(),
        materials.size()
    );
}

void Model::getBuffer(std::vector<float>& vertices, std::vector<unsigned int>& indices) const
{
    vertices.clear();
    indices.clear();

    // 使用映射来避免重复顶点
    std::map<std::tuple<int, int, int>, unsigned int> vertexMap;
    unsigned int currentIndex = 0;

    std::vector<attrib_t> uniqueVertices;

    // 处理每个三角形
    for (const auto& triface : trimesh.trifaces) {
        for (int i = 0; i < 3; i++) {
            const point_t& point = triface.vertex[i];
            
            // 创建顶点键
            auto vertexKey = std::make_tuple(
                point.vertex_index,
                point.normal_index,
                point.texcoord_index
            );
            
            // 检查顶点是否已存在
            auto it = vertexMap.find(vertexKey);
            if (it != vertexMap.end()) {
                // 使用现有索引
                indices.push_back(it->second);
            } else {
                // 创建新顶点
                attrib_t vertexAttrib;
                
                // 位置
                const auto& pos = attrib.vertices[point.vertex_index];
                vertexAttrib.position_ = pos;
                
                // 法线
                const auto& normal = attrib.normals[point.normal_index];
                vertexAttrib.normal_ = normal;
                
                // 纹理坐标
                const auto& texcoord = attrib.texcoords[point.texcoord_index];
                vertexAttrib.texcoord_ = texcoord;
                
                // 添加到映射和数组
                vertexMap[vertexKey] = currentIndex++;
                uniqueVertices.push_back(vertexAttrib);
                indices.push_back(vertexMap[vertexKey]);
            }
        }
    }

    vertices.reserve(uniqueVertices.size() * 8);
    for (const auto& vertex : uniqueVertices) {
        vertices.push_back(vertex.position_.x);
        vertices.push_back(vertex.position_.y);
        vertices.push_back(vertex.position_.z);
        vertices.push_back(vertex.normal_.nx);
        vertices.push_back(vertex.normal_.ny);
        vertices.push_back(vertex.normal_.nz);
        vertices.push_back(vertex.texcoord_.u);
        vertices.push_back(vertex.texcoord_.v);
    }
}