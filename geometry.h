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



//Mat4 struct
struct Mat4 {
    double m[4][4] = {{0.0,0.0,0.0,0.0}, {0.0,0.0,0.0,0.0}, {0.0,0.0,0.0,0.0}, {0.0,0.0,0.0,0.0}};

    friend inline Vec4 operator*(const Mat4& a, const Vec4& b) {
        double res[4];
        for (int i = 0 ; i < 4 ;++i){
            res[i] = a.m[i][0] * b.x + a.m[i][1] * b.y + a.m[i][2] * b.z + a.m[i][3] * b.w;
        }
        return Vec4{res[0],res[1],res[2],res[3]};
    }

    friend Mat4 operator*(const Mat4& a, const Mat4& b) {
        Mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += a.m[i][k] * b.m[k][j];
                }
            }
        }
        return result;
    }
};

inline Mat4 identity(){
    return Mat4{{{1.0,0.0,0.0,0.0}, {0.0,1.0,0.0,0.0}, {0.0,0.0,1.0,0.0}, {0.0,0.0,0.0,1.0}}};
}



//viewport NDC-> 屏幕坐标
//z轴在视野进入屏幕这一维度，方便后面做z-buffer
inline Mat4 viewport(int width,int height){
    Mat4 vp = identity();
    vp.m[0][0] = width / 2.0;
    vp.m[0][3] = width / 2.0;

    vp.m[1][1] = height / 2.0;
    vp.m[1][3] = height / 2.0;

    return vp;
}



//平移
inline Mat4 translation(double tx,double ty,double tz){
    Mat4 tr = identity();
    tr.m[0][3] = tx;
    tr.m[1][3] = ty;
    tr.m[2][3] = tz;

    return tr;
}

//scale放缩
inline Mat4 scale(double sx,double sy,double sz){
    Mat4 sc = identity();
    sc.m[0][0] = sx;
    sc.m[1][1] = sy;
    sc.m[2][2] = sz;

    return sc;
}