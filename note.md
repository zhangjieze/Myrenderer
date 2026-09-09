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

---

# day6
1. 实现tgaimage.h接口,包含TGAColor结构体和TGAImage类

2. 实现tgaimage.cpp,包含对TGAImage的具体实现:
    特别是set的实现
    - 通过坐标(x,y)找到pixel在pixels数组中的存储位置并进行初始化,std::size_t表示无符号数,它的位数大小取决于当前的操作系统和编译器,可以有效防止在分辨率过大时,y\*width_的溢出问题
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


# day18
- 结合颜色插值和深度插值,一段时间没有做了,用于复习和巩固,这只是实现两个属性,后面属性越多越细节,渲染效果更好.这算是一个小 rasterizer 的核心部分.
- 同时现在在函数里面使用的都是vec2,下一步需要把NDC坐标映射为屏幕坐标,提取出xy作为vec2的内容即可,这是从直接的屏幕坐标过渡到图形管线坐标的第一步


# day19
- 实现NDC -> viewport,实现xy()函数提取Vec2
- 进行`model matrix`尝试,完整的管线顺序为local -> model -> world -> view -> clip -> NDC -> viewport -> screen,viewport 是最后把 NDC 变成屏幕坐标的步骤,所以Model等变换都应该发生在几何空间里,而不是屏幕像素空间里.
实现代码为Model_Matrix.cpp,tga文件为NDC.tga,实现两种变换,只进行深度插值,可以看出变换顺序带来的差异


# day20
- 进入 View Matrix
- 实现第一步:**相机平移**
- 相机往右平移,那么物体就是向左平移,即相机📷的平移方向和物体的平移方向是相反的
我自己进行了测试，这里红色我是作为中心，蓝色的摄像头在右边，所以相当于物体往左边动，因此从摄像头看物体的位置就是在蓝色那里，绿色的摄像头在上面，因此绿色相当于向下同理.
实现效果为`view_matirx_translation.tga`



# day21
- 实现第二步:**相机旋转**
- 相机位于原点,发生了旋转,直观上理解,相机的视角发生偏移,那么和平移类似,世界其实是往反方向移动的:相机旋转 +θ,世界则相对相机旋转 -θ
tips:因为旋转矩阵为正交阵,故满足$R^TR=I$ ,$R^-1 = R ^ T$.


# day22
- 相机旋转跟进,不实现直接对矩阵取逆,而是手动实现对于世界的旋转矩阵,规则是:$(AB)^{-1}=B^{-1}A^{-1}$
- 如果相机的旋转是:
    ```cpp
    Mat4 camera_rotation =
    rotation_y(yaw) *
    rotation_x(pitch) *
    rotation_z(roll);
    ```
    则对于世界的旋转则为:
    ```cpp
    Mat4 view_rotation =
    rotation_z(-roll) *
    rotation_x(-pitch) *
    rotation_y(-yaw);
    ```
 
 ---
 - **lookat**,约定3个输入: **1.eye:相机在世界中的位置 2.target:相机要看向的世界点 3.up:“画面上方朝向”的参考方向.**首先,相机的观察方向是相机面向世界点的方向,forward = target - eye.但只有forward还不够.因为“朝着点看”并没有规定相机怎么滚转.
 也就是说相机的其它方向是不固定的,此时up再给一个自由度,告诉我们,理想情况的相机位置它的上方在什么地方.同时可以用叉积确定它的右方向.
 $right=normalize(cross(forward,up))$.
 同时由于up本身并不与forward垂直,所以还需要再进行一次外积得到一个最标准的up.

 因此完整的“相机轴建立”过程是:
 ```cpp
    Vec3 forward   = normalized(target - eye);
    Vec3 right     = normalized(cross(forward, up));
    Vec3 camera_up = cross(right, forward);
 ```
tips:有两个特殊的情况,也就是说如果目标点和相机重合,target == eye,那么就没有相机去看的这么一说,以及如果forward和up平行,叉积就是零向量,无法确定右方向.


- 对于任意一点P,它在相机坐标系里面该如何表示呢?假设它的世界坐标是D,相机坐标是eye,那么如果相机作为原点,表示的向量为P-eye.
也就是差向量.相机坐标的 X 分量，就是P在 right 方向上的投影长度:$x = dot(right,D-eye)$,同理它的上方为$y = dot(camera\_up,D-eye)$.在右手系表示的坐标系中,镜头朝向是 -Z 方向(即右手系的真正z正方向为forward的负方向),所以在右手系表示中$z = dot(-forward,D-eye)$
- 为方便计算,展开$x = dot(right,D) - dot(right,eye)$,最终得到的`View Matrix`为
```
[rx ry rz -dot(right,eye)]
[ux uy uz -dot(camera_up,eye)]
[-fx -fy -fz dot(forward,eye)]
[0 0 0 1]
```
---



# day23
- 修改`View_Matrix_rotation.cpp`中的角度,sin和cos接受的角度是弧度,同时进行冗余代码微调
- 在geometry.h 中 实现`look_at()`内联函数,矩阵前三行分别对应 right、camera_up、-forward 的点积投影,第四列则是把相机原点移到 eye 后得到的平移项.

实现look_at.cpp,从正着的角度看和从斜着的角度机位看,红色三角形对应正面相机,蓝色对应斜向相机.两者的重心都应接近屏幕中心,因为 `target` 是原点,蓝色轮廓的变化来自相机坐标轴改变.
而因为没有实现Project Matrix,所以蓝色三角形不会因为相机更远或更近而产生透视缩小,从而还是有一种奇怪的感觉.


# day24
> View Matrix 部分已经完整收束:平移、旋转的逆变换、look_at 


推导、可复用接口和独立场景都具备,不过为了解决相机的远近造成的影响,进入Projection Matrix.

当前管线为 `local -> model -> look_at(view) -> viewport`
设一个顶点已经经过 Model Matrix 和 View Matrix,处于相机空间 
$P_view = [x_v y_v z_v 1]$,现在乘以Projection Matrix得到其次裁减坐标$P = [x_c y_c z_c w_c]$,这一部分就是合理构造$w_c$,让后续的除法能够产生“近大远小”


1. 第一步:右手相机中,可见点满足: $z < 0$,我们希望透视除法的分母是正的距离,即$w_c = -z_v$,故Matrix的第四行必须是[0 0 -1 0].
```cpp
projection.m[3][0] = 0.0;
projection.m[3][1] = 0.0;
projection.m[3][2] = -1.0;
projection.m[3][3] = 0.0;
```
这样会出现$x_ndc = x_clip / w_clip,y_ndc = y_clip / w_clip$,从而实现距离越远,分母越大,画面越小

2. 第二步:那么拿到裁剪空间的坐标,具体应该如何做? 以大白话角度来描述:首先,画面有一个角度,相机里面的广角类似,视角越广,看见的平面范围就相应的越广;同时看到的画面肯定和原先屏幕的大小有关,也就是width和height;还有就是看到的空间范围肯定不是任何深度都能看到,需要规定能看到的最近的平面和最远的平面.
综上,给出四个参数表述相机的视椎体:
```shell
fovy   ：垂直视场角，单位是弧度
aspect ：画面宽 / 高
near   ：近裁剪平面到相机的正距离
far    ：远裁剪平面到相机的正距离
```

3. 第三步: 对于x,y坐标,最主要需要考虑的为fovy(视场角)和aspect(宽高比),详细见`Projection-xy`.对于深度z,希望 Projection 不会让它依赖 X/Y,即z_c和z呈现线性关系,$z_{clip}=A z_{view}+B$,这里的 $B$ 来自输入点的 w = 1.通过near -> -1和far -> 1可以解出A和B.
矩阵最终结果为`Projection Matrix`
综上,四行的最终职责分别为:
```shell
第 1 行：控制水平视场和 aspect
第 2 行：控制垂直视场
第 3 行：把 near/far 深度映射到 clip
第 4 行：制造 w = -z，供透视除法使用
```
tips: 如果fov是水平视场的时,那么宽高比修正确实应该体现在y方向.
理解: $tan(fov_{x}/2) = width/height * tan(fov_{y}/2)$


# day25
- 实现projection的例子`Projection_Matrix.cpp`,这里实现时需要注意vp不能直接和Projection -> view -> model管线接起来,因为要实现透视除法,中间必须先对 Clip Space 做 / w 变成NDC空间才行.
每个点的实现的过程为`p_local -> p_clip -> p_ndc -> p_screen`.


# day26
- 修改zbuffer规则,因为near -> -1,far -> 1,所以现在变成了`越小越近`,旧的测试作为旧阶段测试,不做修改;修改rasterize.cpp的深度规则:z >= zbuffer[index]的时候证明距离相机越远.
- 进行projection规则下新的深度测试,在`projection_depth.tga`中,红色近三角形覆盖重叠区域.蓝色远三角形只在红色之外的右侧部分可见

- [tips]: to_screen()返回的Vec4.z仍是z_ndc,因为viewport()不改Z;它正好是rasterizer约定要接收的深度值.


# day27
- 属性插值
- 因为在triangle画三角形的时候光栅器先有p_screen:必须反推出对应的 `α、β、γ`才能进行正确的插值,但是传入的a_screen等坐标是经过了projection matrix的,透视投影不是仿射变换  $x_{screen}=\frac{x_{view}}{-z_{view}}$
```zsh
屏幕重心坐标：
描述投影后三角形中的位置

原始三维重心坐标：
描述模型/世界/视图空间三角形中的位置
```
在正交投影或三个顶点 w 相同的情况下,两者可以直接对应;但顶点深度不同,发生透视除法后,它们不再相同,所以需要通过1/w把**屏幕重心坐标变换回原始三维的重心坐标**.

- 透视投影后,原三维三角形的边仍然投影为屏幕上的边;只要三个顶点都在可见区域内,原三角形覆盖的像素区域就是这个屏幕三角形.覆盖测试只关心“形状轮廓在哪里”,不关心像素对应三维表面的精确比例.
- 但颜色、UV、法线等属性要回答的是:这个屏幕像素对应原三维三角形上的哪个位置？这时屏幕重心坐标只能告诉“它在屏幕三角形中的比例”，不能直接告诉“它在原三维表面中的比例”,所以需要 1 / w 校正.

- [tips]: rasterizer只需要回答这个点是否在三角形内部,也就是判断 `bc.x>= 0 && bc.y >= 0 && bc.z >= 0`.


# day28
- 升级插值函数`triangle_barycentric_gradient_depth`,里面用到了颜色的属性,这里有两组重心坐标 screen_bc:覆盖测试和 NDC z 插值;surface_bc:颜色插值,之后也会用于 UV、法线等三维表面属性
- 可能导致一些前面的测试无法编译,后面在遇到的时候进行修改即可.


# day29
- 实现perspective_gradient.cpp


# day30
- 目前能够实现一条比较完整的管线`model -> view -> projection -> vp`
- 现在进入 OBJ loader, OBJ 是一种很简单、很常见的3D模型文件格式,之前做的非洲人头内部点就是用obj文件格式保存的,比如african_head.obj
- OBJ 文件把同类数据写在文本中,是模型数据进入管线的入口
- OBJ最基础的数据结构为v：vertex,顶点位置,f：face,面,顶点位置即为几何空间坐标,f:a b c表示这个这个面由第a个顶点,第b个顶点以及第c个顶点组成
- `vt` 和 `vn` 会分别在纹理、光照阶段接入.
- 实现model.h


# day31
- 实现model.cpp解析obj文件中点的信息

- 补充知识:
```
ifstream为 input file stream输入文件流,是专门用于从文件读取数据的对象,后面就可以从input里面读取内容
后面就是一行一行读取文件
istringstream的作用是把一整行再拆开,从stream里面按空格分开依次读取

整体流程:
OBJ 文件
   ↓
ifstream
   ↓
读取一整行
   ↓
"v 1.2 3.4 5.6"
   ↓
istringstream
   ↓
依次提取
   ↓
v    1.2    3.4    5.6
之后保存进vertice_即可
```
是一个非常简化的 OBJ 文件解析器.后面解析face的信息是类似的.


# day32
- 实现model.cpp解析obj文件中面的信息
- obj中face行的结构为`位置索引 / 纹理坐标索引 / 法线索引`,例如: f 24/1/24 25/2/25 26/3/26,目前只保存这个面的位置索引,在后面引入纹理和法线后更新Face的struct.
- 实现face_vertice_test,测试model.cpp的效果,同时验证索引能取回真正的三角形顶点.


# day33
- 画出african_head的头部线框模型,不过没有引入zbuffer

# day34
- head_zbuffer用`triangle_barycentric_depth`实现纯色zbuffer,不过不能说明zbuffer的作用,需要引入明暗变化
- head_zbuffer 中设计Flat Shading,最基础的 Lambert 漫反射光照,即根据`“面朝向光源的程度”`计算亮度,然后把这个亮度转换成灰度颜色
**tips::** 这里使用的是方向光,不使用点光源,Lambert 漫反射只关心表面能否正对入射光.同时光源很远,所有光线近似平行,因此每个面都可以共用固定光照基准 light_direction = {0, 0, 1}
- Lambert 漫反射公式为I=max(0,n⋅l),即负强度代表背光,直接不亮即可


# day35(UV)
- 进入纹理映射
- 更新tgaimage,实现`get`,get: pixels_[index/index+1/index+2] -> color.b/g/r,为`set`镜像,set: color.b/g/r -> pixels_[index/index+1/index+2]
- **实现`read`**
tga格式文件格式为:
┌──────────────────────┐
│ TGA Header           │  18 bytes
├──────────────────────┤
│ Image ID             │  可选
├──────────────────────┤
│ Color Map            │  可选
├──────────────────────┤
│ Pixel Data           │
│ Pixel Data           │
│ Pixel Data           │
└──────────────────────┘

header用于存头18bytes内容.

- ifstream的read接口为`read(char* buffer, std::streamsize count);`,要求char* 类型,而header中的元素类型是uint8_t*(header.data()返回第一个元素指针),虽然都是字符,但是接口不匹配.reinterpret_cast实现将此uint8_t 内存暂时当成一块 char 内存存储,从而可以让匹配接口让read往这块内存中写内容,最终实现把input的前18字节写入header. 注意这里只是内存上的映射关系,而非数值的转化.

- 读取完需要检查input,检查是否因为header长度不够18byte而读取失败 `if(!input) return false;`

- 接下来解释 TGA Header,以下为tga header的含义
| 字节位置    | 含义                |
| ------- | ----------------- |
| `0`     | Image ID 长度       |
| `1`     | 是否有 Color Map     |
| `2`     | 图片类型              |
| `3~11`  | Color Map / 坐标等信息 |
| `12~13` | 图片宽度              |
| `14~15` | 图片高度              |
| `16`    | 每像素多少 bit         |
| `17`    | Image Descriptor  |

- header[0]记录ImageID的长度,后面用于跳过
- header[1]记录是否使用调色板,也就是Color Mapped Image,不过这里只使用简单BGR不进行颜色编号映射,后期如果对颜色有要求可选,此处仅支持color_map_type == 0.
- TGA支持多种图片编码模式,常见的有2(Uncompressed True-Color Image),3(Uncompressed Grayscale Image),10(RLE True-Color Image),此处支持未压缩的真彩图片,即header[2] == 2
- 宽度采用小端序存储,12存储的是低8位,13为高8位,故需要先将13左移8b,具体就是width = lowB + highB * 256.
- 高度同理.
- bits_per_pixel记录每个像素占多少bit,这里BGR各8b,故为24bits
- header[17]表示布局,当前 framebuffer 和贴图都采用左下角原点,所以暂时只接受 0,避免读入后上下颠倒


# day36
完善对read的说明.
- 对于imageid这里无需使用,所以通过ignore跳过.
- 整个文件一共有width * height * 3个字节,uint8_t = 1 byte,所以创建一个pixels用于存储所有的字节.
- input read方法把Pixel Data读入pixels,如果读取成功,即更新TGAImage的对象.
- 这里的思想是先完整解析到临时状态,验证成功后再 commit 到对象,而不是直接对对象进行修改,这样不会把TGAImage对象改个半残.
tips: 不直接写`pixels_ = pixels;`,因为这样会涉及拷贝,.swap直接交换内存即可,也可以使用stl库的move.



# day37
read读取RLE Image实现.
- RLE采用数据包(packet)格式,packet 开头有一个 1 字节的 packet_header,其内容为:
┌──────────┬─────────────────────┐
│ bit 7    │ bit 6 ~ bit 0       │
├──────────┼─────────────────────┤
│ 类型       packet像素量 - 1
└──────────┴─────────────────────┘
最高bit如果为`1`说明这个这个是RLE packet,是0则说明这是Raw packet.

- `packet_header & 0x7f`把最高位 bit7 清零,只留下低 7 位(packet像素数量-1),因此+1即可算出packet描述的像素数量.
- 一个 RLE packet,同一个像素会重复 count 次”,因此循环count,网pixels中写入count个同类型像素.RLE的作用其实就是实现压缩存储的功能,复用下一个位置的bgr.
- Raw packet是未经过压缩存储,故只需要从前往后读取相应数目(count)的像素即可.
整个RLE解码过程伪代码为:
```
while 还没有填满整张图片(pixel_index < pixel_cnt):

    读取 1 byte packet_header

    从低 7 位得到 count

    得到当前写入位置 destination

    如果 bit7 = 1:
        读取 1 个 BGR
        重复写 count 次

    否则:
        直接读取 count 个 BGR

    pixel_index += count

```



# day38
- 实现对obj解析器对obj纹理坐标(vt)的读取.保存于`texcoords_`中.

思考🤔:因为face不能只保存顶点位置的索引,同时要保存顶点对应uv坐标的索引,所以每个face存两个array,第一个array用于保存位置索引,第二个array用于保存uv索引,后面还有的话以此类推。


# day39
- 不过更好的维护方式是直接定义一个数据结构记录facevertex的位置索引和uv索引更合适.
- 实现parse_face_vertex,用于读取每个面的点的position_index和texcoord_index.










