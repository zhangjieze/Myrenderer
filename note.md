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
    
    NDC坐标:设备归一化坐标，OpenGL为[-1,1],Direct3D为[0,1],这里采用OpenGL标准,映射回原屏幕坐标为:
    screen_x = (ndc_x + 1.0) * width / 2.0;
    screen_y = (ndc_y + 1.0) * height / 2.0;
    z和w需保持不变
    w必须保持为1，设置的viewport矩阵为:
    [width/2,0,0,width/2]
    [0,height/2,0,height/2]
    [0,0,1,0]
    [0,0,0,1]

# day3
1. 实现translation矩阵，可以实现点的平移，是不改变方向的，这里方向不要理解为点的位置向量，指的是两个点之间的差D = B - A，经过平移后两个点之间的差向量是不会因为点的平移改变的。

# day4
1. 实现scale放缩矩阵，可以实现点的位置变换，最主要的是，它可以改变空间中向量的大小和方向
translation：改变点的位置，不改变差向量
uniform scale：改变点的位置和差向量长度，不改变方向
non-uniform scale：改变点的位置，也可能改变差向量方向