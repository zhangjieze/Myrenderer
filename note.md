# day0（6.12）
不要继续堆效果，而是要把模型、数学、管线、着色、输出这几件事拆开。

# day1
实现geometry.h的初步效果:
- 实现最小线性代数层：Vec2 / Vec3 / ==Vec4==
- 实现基本代数运算: 点积(dot),叉积(cross),范数(norm),单位化(normalized)
- 需要特别注意，vec4的第四个维度一般不在规范化考虑范围中，一般是属性的样子，所以vec4有特别的normalized_xyz.
