# day0（6.12）
不要继续堆效果，而是要把模型、数学、管线、着色、输出这几件事拆开。

# day1
实现geometry.h的初步效果:
- 实现最小线性代数层：Vec2 / Vec3 / ==Vec4==
- 实现基本代数运算: 点积(dot),叉积(cross),范数(norm),单位化(normalized)
- 需要特别注意，vec4的第四个维度一般不在规范化考虑范围中，一般是属性的样子，所以vec4有特别的normalized_xyz.

# day2
1. 补足geometry.h的Mat4部分，实现矩阵乘向量和矩阵相乘的基本代数运算
2. 实现Viewport Matrix，把NDC坐标映射到屏幕坐标
    NDC坐标:设备归一化坐标，OpenGL为[-1,1],映射回原屏幕坐标为:
    screen_x = (ndc_x + 1.0) * width / 2.0;
    screen_y = (ndc_y + 1.0) * height / 2.0;
    z和w需保持不变
    w必须保持为1，设置的viewport矩阵为:
    [width/2,0,0,width/2]
    [0,height/2,0,height/2]
    [0,0,1,0]
    [0,0,0,1]