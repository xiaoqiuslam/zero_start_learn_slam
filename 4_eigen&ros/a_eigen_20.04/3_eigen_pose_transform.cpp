#include <iostream>
#include <Eigen/Core>
#include <cmath>
#include <Eigen/Geometry>

int main( int argc, char** argv ){

    // 旋转向量(角轴)（3X1）: Eigen::AngleAxisd
    // 欧拉角（3X1）: Eigen::Vector3d
    // 旋转矩阵（3X3）: Eigen::Matrix3d
    // 四元数（4X1）: Eigen::Quaterniond
    // 平移向量（3X1）: Eigen::Vector3d
    // 变换矩阵（4X4）: Eigen::Isometry3d

    // 1. 旋转向量(角轴)
    // 弧度 = 角度*M_PI/180
    // 角度 = 弧度*180/M_PI
    // M_PI/2, Eigen::Vector3d ( 0,0,1 ) ); //以（0,0,1）z 为旋转轴，旋转45度
    Eigen::AngleAxisd rotation_vector_z(M_PI/4, Eigen::Vector3d ( 0,0,1 ) );
    std::cout << "rotation_vector z " << "angle is: " << rotation_vector_z.angle() * 180 / M_PI
    << " axis is: " << rotation_vector_z.axis().transpose() << std::endl;

    Eigen::AngleAxisd rotation_vector_y(M_PI/4, Eigen::Vector3d ( 0,1,0 ) );
    std::cout << "rotation_vector y " << "angle is: " << rotation_vector_y.angle() * 180 / M_PI
    << " axis is: " << rotation_vector_y.axis().transpose() << std::endl;

    Eigen::AngleAxisd rotation_vector_x(M_PI/4, Eigen::Vector3d ( 1,0,0 ) );
    std::cout << "rotation_vector x " << "angle is: " << rotation_vector_x.angle() * 180 / M_PI
    << " axis is: " << rotation_vector_x.axis().transpose() << std::endl;

    // 2. 旋转向量转化为旋转矩阵
    Eigen::Matrix3d rotation_matrix3d;
    rotation_matrix3d = rotation_vector_z.matrix();
    std::cout<<"rotation_matrix3d \n"<< rotation_matrix3d << std::endl;

    // 3. 旋转矩阵转换为欧拉角,"2" z axis, "1" y axis, "0" x axis,
    Eigen::Vector3d euler_angle_012 = rotation_matrix3d.eulerAngles(0, 1, 2);
    std::cout << "绕Z轴旋转的角度是 " << euler_angle_012(2) * 180 / M_PI << std::endl;
    Eigen::Vector3d euler_angle_210 = rotation_matrix3d.eulerAngles(2, 1, 0);
    std::cout << "绕Z轴旋转的角度是 " << euler_angle_210(0) * 180 / M_PI << std::endl;

    // 4. 欧拉角
    Eigen::Vector3d euler_angle = Eigen::Vector3d(0, 0, M_PI/4);
    std::cout<<"欧拉角: "<< euler_angle.transpose() << std::endl;

    // 5. 欧拉角转角轴
    Eigen::AngleAxisd rollAngle(Eigen::AngleAxisd(euler_angle(0),Eigen::Vector3d::UnitX()));
    Eigen::AngleAxisd pitchAngle(Eigen::AngleAxisd(euler_angle(1),Eigen::Vector3d::UnitY()));
    Eigen::AngleAxisd yawAngle(Eigen::AngleAxisd(euler_angle(2),Eigen::Vector3d::UnitZ()));

    // 6. 角轴转旋转矩阵
    Eigen::Matrix3d rotation_matrix;
    rotation_matrix=yawAngle*pitchAngle*rollAngle;
    std::cout << "rotation_matrix \n" << rotation_matrix <<  std::endl;

    // 7. 旋转向量初始化四元数
    Eigen::Quaterniond quaterniond;
    quaterniond = Eigen::Quaterniond(rotation_vector_z);
    // Eigen四元数初始化赋值顺序是[w,x,y,z]
    // Eigen四元数内部存储时顺序是[x y z w]
    std::cout << "quaterniond2\n" << quaterniond.coeffs().transpose() << std::endl;

    // 8. 旋转矩阵初始化四元数
    Eigen::Quaterniond quaterniond_2 = Eigen::Quaterniond(rotation_matrix3d);
    std::cout << "quaterniond_3\n" << quaterniond_2.coeffs().transpose() << std::endl;

    // 9. 变换矩阵 Eigen::Isometry3d或者Eigen::Matrix4d
    Eigen::Isometry3d transform_matrix = Eigen::Isometry3d::Identity();
    transform_matrix.pretranslate(Eigen::Vector3d(1, 0, 0));
    std::cout << "Transform matrix = \n" << transform_matrix.matrix() << std::endl;

    Eigen::Isometry3d transform_matrix_2 = Eigen::Isometry3d::Identity();
    transform_matrix_2.rotate(rotation_matrix3d);
    transform_matrix_2.pretranslate(Eigen::Vector3d(1, 0, 0));
    std::cout << "transform_matrix_2 = \n" << transform_matrix_2.matrix() << std::endl;

    Eigen::Isometry3d transform_matrix_3 = Eigen::Isometry3d::Identity();
    transform_matrix_3.rotate(quaterniond.normalized().toRotationMatrix());
    std::cout << "transform_matrix_3\n"<<transform_matrix_3.matrix() << std::endl;

    // Eigen::Matrix4d构造变换矩阵,对每一个元素赋值的方法是可行的的,我这里采用的是按矩阵块赋值
    Eigen::Matrix4d T2;
    T2.setIdentity();
    T2.block<3,3>(0,0) = rotation_matrix3d;
    T2.topRightCorner(3, 1) = Eigen::Vector3d(1, 0, 0);

    // 10. 坐标变换
    Eigen::Vector3d vector3d_1(1, 0, 0);
    Eigen::Vector3d vector3d_2;
    vector3d_2 = transform_matrix *  vector3d_1 ;
    std::cout << "vector3d_2: " << vector3d_2.transpose() << std::endl;

    return 0;
}