#include <gtest/gtest.h>
#include <Cedar/Core/String.h>
#include <Cedar/Core/OutOfRangeException.h>
#include <Cedar/Core/Container/List.h>

#include <Cedar/Core/Math/Vector.h>

namespace Cedar::Core {

// Vector2 float
    TEST(Vector3dTest, DefaultConstructor) {
        Math::Vec3d v3d{};
        EXPECT_EQ(v3d[0], 0.0);
    }

    TEST(Vector3dTest, ArgsConstructor) {
        Math::Vec3d v3d(1.0, 3.2, 2.5);
        EXPECT_EQ(v3d[0], 1.0);
        EXPECT_EQ(v3d[1], 3.2);
        EXPECT_EQ(v3d[2], 2.5);
    }

    TEST(Vector3dTest, PositiveOp) {
        Math::Vec3d v3d(-2.4, 6.7, 24.2);

        Math::Vec3d temp = +v3d;

        EXPECT_EQ(temp[0], 2.4);
        EXPECT_EQ(temp[1], 6.7);
        EXPECT_EQ(temp[2], 24.2);
    }

    TEST(Vector3dTest, NegativeOp) {
        Math::Vec3d v3d(-2.4, 6.7, -24.2);

        Math::Vec3d temp = -v3d;

        EXPECT_EQ(temp[0], -2.4);
        EXPECT_EQ(temp[1], -6.7);
        EXPECT_EQ(temp[2], -24.2);
    }

    TEST(Vector3dTest, AddOp) {
        Math::Vec3d v3d_0(-2.4, 6.7, 24.3);
        Math::Vec3d v3d_1(-12.73, -7.31, 337.21);

        Math::Vec3d temp = v3d_0 + v3d_1;

        EXPECT_EQ(temp[0], (-2.4 + -12.73));
        EXPECT_EQ(temp[1], (6.7 + -7.31));
        EXPECT_EQ(temp[2], (24.3 + 337.21));
    }

    TEST(Vector3dTest, SubOp) {
        Math::Vec3d v3d_0(-2.4, 6.7, 24.0);
        Math::Vec3d v3d_1(-12.73, -7.31, 35.1);

        Math::Vec3d temp = v3d_0 - v3d_1;

        EXPECT_EQ(temp[0], (-2.4 - -12.73));
        EXPECT_EQ(temp[1], (6.7 - -7.31));
        EXPECT_EQ(temp[2], (24.0 - 35.1));
    }

    TEST(Vector3dTest, MulOp) {
        Math::Vec3d v3d_0(-2.4, 6.7, 24.0);
        Math::Vec3d v3d_1(-12.73, -7.31, 35.1);

        Math::Vec3d temp = v3d_0 * v3d_1;

        EXPECT_EQ(temp[0], (-2.4 * -12.73));
        EXPECT_EQ(temp[1], (6.7 * -7.31));
        EXPECT_EQ(temp[2], (24.0 * 35.1));
    }

    TEST(Vector3dTest, DivOp) {
        Math::Vec3d v3d_0(-2.4, 6.7, 24.0);
        Math::Vec3d v3d_1(-12.73, -7.31, 35.1);

        Math::Vec3d temp = v3d_0 / v3d_1;

        EXPECT_EQ(temp[0], (-2.4 / -12.73));
        EXPECT_EQ(temp[1], (6.7 / -7.31));
        EXPECT_EQ(temp[2], (24.0 / 35.1));
    }

    TEST(Vector3dTest, EqualOp) {
        Math::Vec3d v3d_0(-2.4, 6.7, 2.4);
        Math::Vec3d v3d_1(-12.73, -7.31, 84.9);
        Boolean temp = v3d_0 == v3d_1;
        EXPECT_EQ(temp, false);

        Math::Vec3d v3d_2(-2.4, 6.7, 25.1);
        Math::Vec3d v3d_3(-2.4, 6.7, 25.1);
        Boolean temp_1 = v3d_2 == v3d_3;
        EXPECT_EQ(temp_1, true);
    }

    TEST(Vector3dTest, InverseEqualOp) {
        Math::Vec3d v3d_0(-2.4, 6.7, 2.4);
        Math::Vec3d v3d_1(-12.73, -7.31, 84.9);
        Boolean temp = v3d_0 != v3d_1;
        EXPECT_EQ(temp, true);

        Math::Vec3d v3d_2(-2.4, 6.7, 25.1);
        Math::Vec3d v3d_3(-2.4, 6.7, 25.1);
        Boolean temp_1 = v3d_2 != v3d_3;
        EXPECT_EQ(temp_1, false);
    }

}
