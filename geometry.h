#pragma once

#include <cmath>

struct Vec2 {
    double x = 0.0;
    double y = 0.0;
    //采用友元函数重载运算符
    friend Vec2 operator+(const Vec2& a, const Vec2& b) { //重载加法运算
        return {a.x + b.x,a.y+b.y};
    }

    friend Vec2 operator-(const Vec2& a, const Vec2& b) { //重载减法运算
        return {a.x - b.x,a.y-b.y};
    }

    friend Vec2 operator*(const Vec2& a, double b) { //重载数乘
        return {a.x * b,a.y * b};
    }

    friend Vec2 operator*(double a, const Vec2& b) {
        return {a * b.x,a * b.y};
    }

    friend Vec2 operator/(const Vec2& a, double b) {
        return {a.x / b,a.y / b};
    }

};

inline double dot(const Vec2& a,const Vec2& b) { //定义点乘
    return a.x * b.x + a.y * b.y;
}

inline double norm(const Vec2& a) {
    return std::sqrt(dot(a, a));
}

inline Vec2 normalized(const Vec2& a) {
    return a / norm(a);
}







//vec3
struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    friend Vec3 operator+(const Vec3& a, const Vec3& b) {
        return {a.x + b.x,a.y+b.y,a.z+b.z};
    }

    friend Vec3 operator-(const Vec3& a,const Vec3& b) {
        return {a.x - b.x,a.y-b.y,a.z-b.z};
    }

    friend Vec3 operator*(const Vec3& a, double b) {
        return {a.x * b,a.y * b,a.z * b};
    }

    friend Vec3 operator*(double a, const Vec3& b) {
        return {a * b.x,a * b.y,a * b.z};
    }

    friend Vec3 operator/(const Vec3& a, double b) {
        return {a.x / b,a.y / b,a.z / b};
    }
};

inline double dot(const Vec3& a,const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
//叉积运算
inline Vec3 cross(const Vec3& a,const Vec3& b) {
    return {a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x};
}
//范数
inline double norm(const Vec3& a) {
    return std::sqrt(dot(a, a));
}
//单位化
inline Vec3 normalized(const Vec3& a) {
    return a / norm(a);
}








//Vec4 struct
struct Vec4 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double w = 0.0;

    friend Vec4 operator+(const Vec4& a, const Vec4& b) {
        return {a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w};
    }

    friend Vec4 operator-(const Vec4& a, const Vec4& b) {
        return {a.x-b.x,a.y-b.y,a.z-b.z,a.w-b.w};
    }

    friend Vec4 operator*(const Vec4& a, double b) {
        return {a.x*b,a.y*b,a.z*b,a.w*b};
    }

    friend Vec4 operator*(double a, const Vec4& b) {
        return {a*b.x,a*b.y,a*b.z,a*b.w};
    }
    
    friend Vec4 operator/(const Vec4& a, double b) {
        return {a.x/b,a.y/b,a.z/b,a.w/b};
    }

};

inline double dot(const Vec4& a,const Vec4& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline double norm(const Vec4& a) {
    return std::sqrt(dot(a, a));
}

inline Vec3 xyz(const Vec4& a) {
    return {a.x,a.y,a.z};
}

inline Vec4 normalized(const Vec4& a) {
    return a / norm(a);
}

inline Vec4 normalized_xyz(const Vec4& a) {
    double len = norm(xyz(a));
    return {a.x / len,a.y / len,a.z / len,a.w};
}