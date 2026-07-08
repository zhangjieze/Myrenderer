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

# day5
1. 实现rotation旋转，绕z轴逆时针旋转，旋转矩阵为:
    [ cos  -sin   0   0 ]
    [ sin   cos   0   0 ]
    [ 0      0    1   0 ]
    [ 0      0    0   1 ]
    同理可以实现绕x,y轴旋转,不过注意在绕y轴时因使用右手系,旋转矩阵为:
    [ cos   0   sin   0 ]
    [ 0     1    0    0 ]
    [-sin   0   cos   0 ]
    [ 0     0    0    1 ]

2. 理解变换组合逻辑:顺序其实就是左边的变换矩阵对列向量做的空间的仿射变换,仿射变换的顺序直接对仿射变换的结果影响


# day6
1. 实现tgaimage.h接口,包含TGAColor结构体和TGAImage类

2. 实现tgaimage.cpp,包含对TGAImage的具体实现:
    特别是set的实现
    - 通过坐标(x,y)找到pixel在pixels数组中的存储位置并进行初始化,std::size_t表示无符号数,它的位数大小取决于当前的操作系统和编译器,可以有效防止在分辨率过大时,y*width_的溢出问题
    - static_cast是cpp中显式强制类型转换,更安全

3. 实现TGAImage::write,其中:
    |> 头文件信息
    - TGA 格式的官方规范规定，文件开头的 18 个字节必须用来存储图像的元数据,同时width和height的高度限制由于TGA的2字节(16bit)存储字段,故高度和宽度只能达到65535
    - header[2] = 2;：根据 TGA 规范，值 2 代表这是一张“未压缩的真彩色图像（Uncompressed, true-color image）”
    - header[16] = 24;：代表每像素占用 24 位(即 3 个通道 3 x 8 = 24 bit)
    - 因为 width_ 和 height_ 是32 位的整数（占 4 字节），但 TGA 头文件里只给宽度分配了 2 个字节的空间（第 12 和 13 字节），且要求以**小端序（即低位字节在前，高位字节在后）**写入,故先通过按位与操作取出低8位存入12,在右移8位取出新的低8位

    |> 输出流
    - std::ofstream是 C++ 中的“输出文件流”，专门用于向文件中写入数据
    - std::ios::binary是文件打开格式，关闭一切自动转换，而一般文件打开是用的文本格式，这样文件可能会被系统改变
    - 这里的output.write函数原型是: ostream& write(const char* s, std::streamsize count);
    - 但是传入的数据是std::uint8_t,因此 header.data() 返回的是 const uint8_t* 类型的指针。但是，C++ 历史遗留的文件写入函数 write() 只接受 const char*（有符号字符）指针,故使用reinterpret_cast<const char*>(重新解释类型转换),不用做任何实质性的数据转换，直接把这个 uint8_t* 指针当成 char* 指针来用
    - header.size() 返回的是 std::size_t 类型,而 write 函数需要的长度参数类型是 std::streamsize（标准库定义的一种有符号整型）,为了防止编译器报“有符号/无符号类型不匹配”的警告，这里使用 static_cast 进行了一次类型转换
    - good() 是文件流的一个状态检查函数。如果整个写入过程中没有发生任何硬件或系统错误，它会返回 true，代表文件保存成功


# day7
- test.cpp文件用于后续测试
使用tga底层文件实现画点,输出output.tga，可执行文件为test1

# day8
- 实现最基本的划线算法---DDA算法,让变化最大的坐标轴每次大约移动一个像素，避免出现空隙,所以取max
- 最基本的画线实现,可执行文件为test2,生成的tga文件为day8line.tga

tips:
- inline函数的完整定义必须在每个使用它的翻译单元中可见,所以一般是直接定义,而如果是普通函数，则使用.h:声明 .cpp:实现的方式

# day9
创建rasterizer.h和rasterizer.cpp,line不属于tgaimage的范畴.rasterizer用于实现光栅化算法

# day10
实现初步的Bresenham直线算法.
- 每走一步 x，都判断 y 要不要加 1,因为理想的情况是以小数往上不断加(y_real = y0 + slope * (x - x0)) , 但是但屏幕像素只能取整数 y,算法的基本思想是维护一个误差值error,如果误差超过一半的dx时,让 y加1,同时回正error

数学原理:
```
直线的方程是y = y0 + (dy / dx) * (x - x0),因为屏幕像素只能取整数,因为斜率不超过 1, 所以这里把 x 当成主变量(dx > dy),所以在 x+1 之后,y到底什么时候需要+1? 
每走一步 x，真实直线的 y 会增加 dy/dx(这里之所以dx在下也是因为斜率小于等于1,所以dx是更长的边,相当于dda里面的steps), 这里不要小数,所以把这里增加的误差整体乘以 dx==(相当于把纵轴拉长了dx)==, 所以error增加的就是dy, 当超过一半纵向像素大小,也就是 dx / 2后,这里因为不要出现小数,判断条件为 error * 2 >= dx
y就需要自增1,同时回正error,让error 减去一个dx.
```

bresenham 比dda更光栅化的地方在于它 `不用除法,不用浮点数,不用 round,只用整数加减和比较`.
先实现最基本受限的bresenham算法,x0 <= x1, 0 <= slope <= 1


# day11
Bresenham算法的完善,处理3个问题,`x0 < x1 如何修复? 负斜率如何计算? 过高的斜率(也就是dx < dy)如何计算?`

- `x0 < x1`
如果反向,直接把两个点交换即可,不过注意dx和dy的计算必须在交换之后

- `负斜率`
负斜率的时候相当于y往下走,用一个`ystep`变量保存y需要走的间隔,如果是正斜率,那么y走1,负斜率走-1即可

- `高斜率(dx < dy)`
此时取点应该由y轴取,用一个steep判断dx和dy的绝对值大小,如果dx小,那么交换坐标轴即可,在真正画点的时候再交换回来

最后运行对于bresenham的最终测试,可执行文件为bresenham_final,得到最后的bresenham_final_test.tga文件.



# day12
- 实现三角形轮廓画法(3次Bresenham画线即可),下一步进入三角形填充
分为两个实现方式:
- 扫描线填充
- 重心坐标填充`要用到包围盒,因为点的范围必须固定,直接用一个矩形把三角形包起来然后在用重心推导即可(重心是渲染里面很重要的一个属性,很多别的二次属性都可以由重心提供,比如zbuffer和uv)`


# day13
- 实现扫描线填充方式
需要注意的是在划水平线的过程中不要直接调用line_bresenham,直接水平画线会更高效


# day 14
不再按扫描线找左右边界,而是在三角形包围盒里逐像素判断:这个像素是否在三角形内部
三角形的3个顶点分别为A,B,C,如果P = alpha * A + beta * B + gamma * C,且alpha + beta + gamma = 1,则称 alpha, beta, gamma 就是 P 关于三角形 ABC 的重心坐标。

- `barycentric`实现找一个点关于某个三角形的重心坐标,数学原理为克莱姆法则求解二元方程: P=αA+βB+(1−α−β)C , 则 P−C=α(A−C)+β(B−C)
重心坐标十分重要,很多属性都可以使用重心坐标进行插值得到:
```bash
z = z0 * bc.x + z1 * bc.y + z2 * bc.z;
uv = uv0 * bc.x + uv1 * bc.y + uv2 * bc.z;
normal = n0 * bc.x + n1 * bc.y + n2 * bc.z;
```

- 实现`triangle_barycentric_filled`,使用包围盒,判断包围盒内的点重心坐标是否均大于等于0

## 实现思路：
 - 求三角形包围盒
 - 遍历包围盒里的每个像素
 - 算当前像素点的重心坐标
 - 如果三个权重都非负，就染色

- 测试得到triangle_barycentric.tga



# day15
> 重心坐标应用-颜色插值
- 三角形颜色插值,不过因为没有重载TGAColor的\*运算,所以需要定义一个辅助函数(`to_byte`)实现TGAColor的插值
- 定义新函数`triangle_barycentric_gradient`,在位置插值的基础上增加颜色插值

测试得到效果为triangle_barycentric_color_gradient.tga文件,很明显可以看到越靠近顶点的像素对应这个顶点的颜色越深


# day16
> 重心坐标应用-深度插值zbuffer
- 使用一个vector记录整个image每个像素位置的深度值,规定越**靠近**摄像头的z值越大,zbuffer初始化为负无穷`std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::infinity());`.
- 函数实现为`triangle_barycentric_depth`, 实现效果为triangle\_barycentric\_depth.tga,红色压住了绿色三角形的一部分,同时绿色也压住了红色三角形的一部分,体现各个位置深度的影响



# day17
- 对于包围盒重复代码进行整理,同时rasterizer.h只暴露接口,辅助函数和只在rasterizer.cpp中实现的函数使用namespace隔离
- barycentric() 现在还放在头文件公开 API 里.它是一个有意义且可能被外部测试复用的数学工具,所以接口还是暴露在外









