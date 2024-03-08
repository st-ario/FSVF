/* generated using generate_test_shuffles.py (different approach than the
   one used to create the shuffles in code, to avoid accidentally creating
   testing blindsposts)*/

// clang-format off

TEST_CASE("ShufflesVec4", "[shuffle]")
{
  Vec4 vec(-1.0f, 2.66669667f, 3.4567896f, -4e-4f);

  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzwy>(vec), -4e-4f, 3.4567896f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzww>(vec), -4e-4f, 3.4567896f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzwx>(vec), -4e-4f, 3.4567896f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzwz>(vec), -4e-4f, 3.4567896f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzzy>(vec), -4e-4f, 3.4567896f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzzw>(vec), -4e-4f, 3.4567896f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzzx>(vec), -4e-4f, 3.4567896f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzzz>(vec), -4e-4f, 3.4567896f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzyy>(vec), -4e-4f, 3.4567896f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzyw>(vec), -4e-4f, 3.4567896f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzyx>(vec), -4e-4f, 3.4567896f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzyz>(vec), -4e-4f, 3.4567896f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzxy>(vec), -4e-4f, 3.4567896f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzxw>(vec), -4e-4f, 3.4567896f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzxx>(vec), -4e-4f, 3.4567896f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wzxz>(vec), -4e-4f, 3.4567896f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wywy>(vec), -4e-4f, 2.66669667f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyww>(vec), -4e-4f, 2.66669667f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wywx>(vec), -4e-4f, 2.66669667f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wywz>(vec), -4e-4f, 2.66669667f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyzy>(vec), -4e-4f, 2.66669667f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyzw>(vec), -4e-4f, 2.66669667f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyzx>(vec), -4e-4f, 2.66669667f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyzz>(vec), -4e-4f, 2.66669667f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyyy>(vec), -4e-4f, 2.66669667f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyyw>(vec), -4e-4f, 2.66669667f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyyx>(vec), -4e-4f, 2.66669667f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyyz>(vec), -4e-4f, 2.66669667f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyxy>(vec), -4e-4f, 2.66669667f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyxw>(vec), -4e-4f, 2.66669667f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyxx>(vec), -4e-4f, 2.66669667f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wyxz>(vec), -4e-4f, 2.66669667f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxwy>(vec), -4e-4f, -1.0f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxww>(vec), -4e-4f, -1.0f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxwx>(vec), -4e-4f, -1.0f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxwz>(vec), -4e-4f, -1.0f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxzy>(vec), -4e-4f, -1.0f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxzw>(vec), -4e-4f, -1.0f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxzx>(vec), -4e-4f, -1.0f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxzz>(vec), -4e-4f, -1.0f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxyy>(vec), -4e-4f, -1.0f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxyw>(vec), -4e-4f, -1.0f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxyx>(vec), -4e-4f, -1.0f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxyz>(vec), -4e-4f, -1.0f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxxy>(vec), -4e-4f, -1.0f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxxw>(vec), -4e-4f, -1.0f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxxx>(vec), -4e-4f, -1.0f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wxxz>(vec), -4e-4f, -1.0f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwwy>(vec), -4e-4f, -4e-4f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwww>(vec), -4e-4f, -4e-4f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwwx>(vec), -4e-4f, -4e-4f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwwz>(vec), -4e-4f, -4e-4f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwzy>(vec), -4e-4f, -4e-4f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwzw>(vec), -4e-4f, -4e-4f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwzx>(vec), -4e-4f, -4e-4f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwzz>(vec), -4e-4f, -4e-4f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwyy>(vec), -4e-4f, -4e-4f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwyw>(vec), -4e-4f, -4e-4f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwyx>(vec), -4e-4f, -4e-4f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwyz>(vec), -4e-4f, -4e-4f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwxy>(vec), -4e-4f, -4e-4f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwxw>(vec), -4e-4f, -4e-4f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwxx>(vec), -4e-4f, -4e-4f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::wwxz>(vec), -4e-4f, -4e-4f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzwy>(vec), -1.0f, 3.4567896f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzww>(vec), -1.0f, 3.4567896f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzwx>(vec), -1.0f, 3.4567896f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzwz>(vec), -1.0f, 3.4567896f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzzy>(vec), -1.0f, 3.4567896f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzzw>(vec), -1.0f, 3.4567896f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzzx>(vec), -1.0f, 3.4567896f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzzz>(vec), -1.0f, 3.4567896f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzyy>(vec), -1.0f, 3.4567896f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzyw>(vec), -1.0f, 3.4567896f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzyx>(vec), -1.0f, 3.4567896f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzyz>(vec), -1.0f, 3.4567896f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzxy>(vec), -1.0f, 3.4567896f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzxw>(vec), -1.0f, 3.4567896f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzxx>(vec), -1.0f, 3.4567896f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xzxz>(vec), -1.0f, 3.4567896f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xywy>(vec), -1.0f, 2.66669667f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyww>(vec), -1.0f, 2.66669667f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xywx>(vec), -1.0f, 2.66669667f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xywz>(vec), -1.0f, 2.66669667f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyzy>(vec), -1.0f, 2.66669667f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyzw>(vec), -1.0f, 2.66669667f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyzx>(vec), -1.0f, 2.66669667f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyzz>(vec), -1.0f, 2.66669667f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyyy>(vec), -1.0f, 2.66669667f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyyw>(vec), -1.0f, 2.66669667f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyyx>(vec), -1.0f, 2.66669667f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyyz>(vec), -1.0f, 2.66669667f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyxy>(vec), -1.0f, 2.66669667f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyxw>(vec), -1.0f, 2.66669667f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyxx>(vec), -1.0f, 2.66669667f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xyxz>(vec), -1.0f, 2.66669667f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxwy>(vec), -1.0f, -1.0f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxww>(vec), -1.0f, -1.0f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxwx>(vec), -1.0f, -1.0f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxwz>(vec), -1.0f, -1.0f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxzy>(vec), -1.0f, -1.0f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxzw>(vec), -1.0f, -1.0f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxzx>(vec), -1.0f, -1.0f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxzz>(vec), -1.0f, -1.0f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxyy>(vec), -1.0f, -1.0f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxyw>(vec), -1.0f, -1.0f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxyx>(vec), -1.0f, -1.0f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxyz>(vec), -1.0f, -1.0f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxxy>(vec), -1.0f, -1.0f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxxw>(vec), -1.0f, -1.0f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxxx>(vec), -1.0f, -1.0f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xxxz>(vec), -1.0f, -1.0f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwwy>(vec), -1.0f, -4e-4f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwww>(vec), -1.0f, -4e-4f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwwx>(vec), -1.0f, -4e-4f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwwz>(vec), -1.0f, -4e-4f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwzy>(vec), -1.0f, -4e-4f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwzw>(vec), -1.0f, -4e-4f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwzx>(vec), -1.0f, -4e-4f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwzz>(vec), -1.0f, -4e-4f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwyy>(vec), -1.0f, -4e-4f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwyw>(vec), -1.0f, -4e-4f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwyx>(vec), -1.0f, -4e-4f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwyz>(vec), -1.0f, -4e-4f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwxy>(vec), -1.0f, -4e-4f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwxw>(vec), -1.0f, -4e-4f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwxx>(vec), -1.0f, -4e-4f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::xwxz>(vec), -1.0f, -4e-4f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzwy>(vec), 3.4567896f, 3.4567896f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzww>(vec), 3.4567896f, 3.4567896f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzwx>(vec), 3.4567896f, 3.4567896f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzwz>(vec), 3.4567896f, 3.4567896f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzzy>(vec), 3.4567896f, 3.4567896f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzzw>(vec), 3.4567896f, 3.4567896f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzzx>(vec), 3.4567896f, 3.4567896f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzzz>(vec), 3.4567896f, 3.4567896f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzyy>(vec), 3.4567896f, 3.4567896f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzyw>(vec), 3.4567896f, 3.4567896f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzyx>(vec), 3.4567896f, 3.4567896f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzyz>(vec), 3.4567896f, 3.4567896f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzxy>(vec), 3.4567896f, 3.4567896f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzxw>(vec), 3.4567896f, 3.4567896f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzxx>(vec), 3.4567896f, 3.4567896f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zzxz>(vec), 3.4567896f, 3.4567896f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zywy>(vec), 3.4567896f, 2.66669667f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyww>(vec), 3.4567896f, 2.66669667f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zywx>(vec), 3.4567896f, 2.66669667f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zywz>(vec), 3.4567896f, 2.66669667f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyzy>(vec), 3.4567896f, 2.66669667f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyzw>(vec), 3.4567896f, 2.66669667f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyzx>(vec), 3.4567896f, 2.66669667f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyzz>(vec), 3.4567896f, 2.66669667f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyyy>(vec), 3.4567896f, 2.66669667f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyyw>(vec), 3.4567896f, 2.66669667f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyyx>(vec), 3.4567896f, 2.66669667f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyyz>(vec), 3.4567896f, 2.66669667f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyxy>(vec), 3.4567896f, 2.66669667f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyxw>(vec), 3.4567896f, 2.66669667f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyxx>(vec), 3.4567896f, 2.66669667f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zyxz>(vec), 3.4567896f, 2.66669667f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxwy>(vec), 3.4567896f, -1.0f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxww>(vec), 3.4567896f, -1.0f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxwx>(vec), 3.4567896f, -1.0f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxwz>(vec), 3.4567896f, -1.0f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxzy>(vec), 3.4567896f, -1.0f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxzw>(vec), 3.4567896f, -1.0f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxzx>(vec), 3.4567896f, -1.0f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxzz>(vec), 3.4567896f, -1.0f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxyy>(vec), 3.4567896f, -1.0f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxyw>(vec), 3.4567896f, -1.0f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxyx>(vec), 3.4567896f, -1.0f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxyz>(vec), 3.4567896f, -1.0f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxxy>(vec), 3.4567896f, -1.0f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxxw>(vec), 3.4567896f, -1.0f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxxx>(vec), 3.4567896f, -1.0f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zxxz>(vec), 3.4567896f, -1.0f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwwy>(vec), 3.4567896f, -4e-4f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwww>(vec), 3.4567896f, -4e-4f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwwx>(vec), 3.4567896f, -4e-4f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwwz>(vec), 3.4567896f, -4e-4f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwzy>(vec), 3.4567896f, -4e-4f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwzw>(vec), 3.4567896f, -4e-4f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwzx>(vec), 3.4567896f, -4e-4f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwzz>(vec), 3.4567896f, -4e-4f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwyy>(vec), 3.4567896f, -4e-4f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwyw>(vec), 3.4567896f, -4e-4f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwyx>(vec), 3.4567896f, -4e-4f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwyz>(vec), 3.4567896f, -4e-4f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwxy>(vec), 3.4567896f, -4e-4f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwxw>(vec), 3.4567896f, -4e-4f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwxx>(vec), 3.4567896f, -4e-4f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::zwxz>(vec), 3.4567896f, -4e-4f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzwy>(vec), 2.66669667f, 3.4567896f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzww>(vec), 2.66669667f, 3.4567896f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzwx>(vec), 2.66669667f, 3.4567896f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzwz>(vec), 2.66669667f, 3.4567896f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzzy>(vec), 2.66669667f, 3.4567896f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzzw>(vec), 2.66669667f, 3.4567896f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzzx>(vec), 2.66669667f, 3.4567896f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzzz>(vec), 2.66669667f, 3.4567896f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzyy>(vec), 2.66669667f, 3.4567896f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzyw>(vec), 2.66669667f, 3.4567896f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzyx>(vec), 2.66669667f, 3.4567896f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzyz>(vec), 2.66669667f, 3.4567896f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzxy>(vec), 2.66669667f, 3.4567896f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzxw>(vec), 2.66669667f, 3.4567896f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzxx>(vec), 2.66669667f, 3.4567896f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yzxz>(vec), 2.66669667f, 3.4567896f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yywy>(vec), 2.66669667f, 2.66669667f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyww>(vec), 2.66669667f, 2.66669667f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yywx>(vec), 2.66669667f, 2.66669667f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yywz>(vec), 2.66669667f, 2.66669667f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyzy>(vec), 2.66669667f, 2.66669667f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyzw>(vec), 2.66669667f, 2.66669667f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyzx>(vec), 2.66669667f, 2.66669667f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyzz>(vec), 2.66669667f, 2.66669667f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyyy>(vec), 2.66669667f, 2.66669667f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyyw>(vec), 2.66669667f, 2.66669667f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyyx>(vec), 2.66669667f, 2.66669667f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyyz>(vec), 2.66669667f, 2.66669667f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyxy>(vec), 2.66669667f, 2.66669667f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyxw>(vec), 2.66669667f, 2.66669667f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyxx>(vec), 2.66669667f, 2.66669667f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yyxz>(vec), 2.66669667f, 2.66669667f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxwy>(vec), 2.66669667f, -1.0f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxww>(vec), 2.66669667f, -1.0f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxwx>(vec), 2.66669667f, -1.0f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxwz>(vec), 2.66669667f, -1.0f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxzy>(vec), 2.66669667f, -1.0f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxzw>(vec), 2.66669667f, -1.0f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxzx>(vec), 2.66669667f, -1.0f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxzz>(vec), 2.66669667f, -1.0f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxyy>(vec), 2.66669667f, -1.0f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxyw>(vec), 2.66669667f, -1.0f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxyx>(vec), 2.66669667f, -1.0f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxyz>(vec), 2.66669667f, -1.0f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxxy>(vec), 2.66669667f, -1.0f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxxw>(vec), 2.66669667f, -1.0f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxxx>(vec), 2.66669667f, -1.0f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::yxxz>(vec), 2.66669667f, -1.0f, -1.0f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywwy>(vec), 2.66669667f, -4e-4f, -4e-4f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywww>(vec), 2.66669667f, -4e-4f, -4e-4f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywwx>(vec), 2.66669667f, -4e-4f, -4e-4f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywwz>(vec), 2.66669667f, -4e-4f, -4e-4f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywzy>(vec), 2.66669667f, -4e-4f, 3.4567896f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywzw>(vec), 2.66669667f, -4e-4f, 3.4567896f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywzx>(vec), 2.66669667f, -4e-4f, 3.4567896f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywzz>(vec), 2.66669667f, -4e-4f, 3.4567896f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywyy>(vec), 2.66669667f, -4e-4f, 2.66669667f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywyw>(vec), 2.66669667f, -4e-4f, 2.66669667f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywyx>(vec), 2.66669667f, -4e-4f, 2.66669667f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywyz>(vec), 2.66669667f, -4e-4f, 2.66669667f, 3.4567896f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywxy>(vec), 2.66669667f, -4e-4f, -1.0f, 2.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywxw>(vec), 2.66669667f, -4e-4f, -1.0f, -4e-4f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywxx>(vec), 2.66669667f, -4e-4f, -1.0f, -1.0f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ywxz>(vec), 2.66669667f, -4e-4f, -1.0f, 3.4567896f);
}

/*
TEST_CASE("ShufflesVec3", "[shuffle]")
{
  LVec3 vec(-1.0f, 2.66669667f, 3.4567896f);

  {
    LVec3 shuffled = shuffle<Shuf3::xzy>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, 3.4567896f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, 3.4567896f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xzx>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, 3.4567896f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, 3.4567896f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xzz>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, 3.4567896f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, 3.4567896f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xyy>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, 2.66669667f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, 2.66669667f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xyx>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, 2.66669667f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, 2.66669667f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xyz>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, 2.66669667f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, 2.66669667f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xxy>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, -1.0f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, -1.0f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xxx>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, -1.0f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, -1.0f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::xxz>(vec);
    FSFV_CHECK_COMP3(shuffled, -1.0f, -1.0f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -1.0f, -1.0f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zzy>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, 3.4567896f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, 3.4567896f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zzx>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, 3.4567896f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, 3.4567896f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zzz>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, 3.4567896f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, 3.4567896f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zyy>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, 2.66669667f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, 2.66669667f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zyx>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, 2.66669667f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, 2.66669667f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zyz>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, 2.66669667f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, 2.66669667f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zxy>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, -1.0f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, -1.0f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zxx>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, -1.0f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, -1.0f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::zxz>(vec);
    FSFV_CHECK_COMP3(shuffled, 3.4567896f, -1.0f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 3.4567896f, -1.0f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yzy>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, 3.4567896f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, 3.4567896f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yzx>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, 3.4567896f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, 3.4567896f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yzz>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, 3.4567896f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, 3.4567896f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yyy>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, 2.66669667f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, 2.66669667f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yyx>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, 2.66669667f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, 2.66669667f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yyz>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, 2.66669667f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, 2.66669667f, 3.4567896f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yxy>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, -1.0f, 2.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, -1.0f, 2.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yxx>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, -1.0f, -1.0f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, -1.0f, -1.0f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::yxz>(vec);
    FSFV_CHECK_COMP3(shuffled, 2.66669667f, -1.0f, 3.4567896f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), 2.66669667f, -1.0f, 3.4567896f, 0);
  }
}
*/

#ifdef USE_RGBA_COMPONENTS
TEST_CASE("ShufflesVec4RGBA", "[shuffle]")
{
  constexpr float pi{3.141592653f};
  constexpr float inv_pi{1.0f/pi};

  Vec4 vec(pi, -.66669667f, -inv_pi, 4e7f);

  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abag>(vec), 4e7f, -inv_pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abaa>(vec), 4e7f, -inv_pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abar>(vec), 4e7f, -inv_pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abab>(vec), 4e7f, -inv_pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abbg>(vec), 4e7f, -inv_pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abba>(vec), 4e7f, -inv_pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abbr>(vec), 4e7f, -inv_pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abbb>(vec), 4e7f, -inv_pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abgg>(vec), 4e7f, -inv_pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abga>(vec), 4e7f, -inv_pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abgr>(vec), 4e7f, -inv_pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abgb>(vec), 4e7f, -inv_pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abrg>(vec), 4e7f, -inv_pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abra>(vec), 4e7f, -inv_pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abrr>(vec), 4e7f, -inv_pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::abrb>(vec), 4e7f, -inv_pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agag>(vec), 4e7f, -.66669667f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agaa>(vec), 4e7f, -.66669667f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agar>(vec), 4e7f, -.66669667f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agab>(vec), 4e7f, -.66669667f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agbg>(vec), 4e7f, -.66669667f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agba>(vec), 4e7f, -.66669667f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agbr>(vec), 4e7f, -.66669667f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agbb>(vec), 4e7f, -.66669667f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aggg>(vec), 4e7f, -.66669667f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agga>(vec), 4e7f, -.66669667f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aggr>(vec), 4e7f, -.66669667f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aggb>(vec), 4e7f, -.66669667f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agrg>(vec), 4e7f, -.66669667f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agra>(vec), 4e7f, -.66669667f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agrr>(vec), 4e7f, -.66669667f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::agrb>(vec), 4e7f, -.66669667f, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arag>(vec), 4e7f, pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::araa>(vec), 4e7f, pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arar>(vec), 4e7f, pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arab>(vec), 4e7f, pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arbg>(vec), 4e7f, pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arba>(vec), 4e7f, pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arbr>(vec), 4e7f, pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arbb>(vec), 4e7f, pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::argg>(vec), 4e7f, pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arga>(vec), 4e7f, pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::argr>(vec), 4e7f, pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::argb>(vec), 4e7f, pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arrg>(vec), 4e7f, pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arra>(vec), 4e7f, pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arrr>(vec), 4e7f, pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::arrb>(vec), 4e7f, pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aaag>(vec), 4e7f, 4e7f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aaaa>(vec), 4e7f, 4e7f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aaar>(vec), 4e7f, 4e7f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aaab>(vec), 4e7f, 4e7f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aabg>(vec), 4e7f, 4e7f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aaba>(vec), 4e7f, 4e7f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aabr>(vec), 4e7f, 4e7f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aabb>(vec), 4e7f, 4e7f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aagg>(vec), 4e7f, 4e7f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aaga>(vec), 4e7f, 4e7f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aagr>(vec), 4e7f, 4e7f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aagb>(vec), 4e7f, 4e7f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aarg>(vec), 4e7f, 4e7f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aara>(vec), 4e7f, 4e7f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aarr>(vec), 4e7f, 4e7f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::aarb>(vec), 4e7f, 4e7f, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbag>(vec), pi, -inv_pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbaa>(vec), pi, -inv_pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbar>(vec), pi, -inv_pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbab>(vec), pi, -inv_pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbbg>(vec), pi, -inv_pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbba>(vec), pi, -inv_pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbbr>(vec), pi, -inv_pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbbb>(vec), pi, -inv_pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbgg>(vec), pi, -inv_pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbga>(vec), pi, -inv_pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbgr>(vec), pi, -inv_pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbgb>(vec), pi, -inv_pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbrg>(vec), pi, -inv_pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbra>(vec), pi, -inv_pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbrr>(vec), pi, -inv_pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rbrb>(vec), pi, -inv_pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgag>(vec), pi, -.66669667f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgaa>(vec), pi, -.66669667f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgar>(vec), pi, -.66669667f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgab>(vec), pi, -.66669667f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgbg>(vec), pi, -.66669667f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgba>(vec), pi, -.66669667f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgbr>(vec), pi, -.66669667f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgbb>(vec), pi, -.66669667f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rggg>(vec), pi, -.66669667f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgga>(vec), pi, -.66669667f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rggr>(vec), pi, -.66669667f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rggb>(vec), pi, -.66669667f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgrg>(vec), pi, -.66669667f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgra>(vec), pi, -.66669667f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgrr>(vec), pi, -.66669667f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rgrb>(vec), pi, -.66669667f, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrag>(vec), pi, pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rraa>(vec), pi, pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrar>(vec), pi, pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrab>(vec), pi, pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrbg>(vec), pi, pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrba>(vec), pi, pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrbr>(vec), pi, pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrbb>(vec), pi, pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrgg>(vec), pi, pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrga>(vec), pi, pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrgr>(vec), pi, pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrgb>(vec), pi, pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrrg>(vec), pi, pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrra>(vec), pi, pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrrr>(vec), pi, pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rrrb>(vec), pi, pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::raag>(vec), pi, 4e7f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::raaa>(vec), pi, 4e7f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::raar>(vec), pi, 4e7f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::raab>(vec), pi, 4e7f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rabg>(vec), pi, 4e7f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::raba>(vec), pi, 4e7f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rabr>(vec), pi, 4e7f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rabb>(vec), pi, 4e7f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ragg>(vec), pi, 4e7f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::raga>(vec), pi, 4e7f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ragr>(vec), pi, 4e7f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ragb>(vec), pi, 4e7f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rarg>(vec), pi, 4e7f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rara>(vec), pi, 4e7f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rarr>(vec), pi, 4e7f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::rarb>(vec), pi, 4e7f, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbag>(vec), -inv_pi, -inv_pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbaa>(vec), -inv_pi, -inv_pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbar>(vec), -inv_pi, -inv_pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbab>(vec), -inv_pi, -inv_pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbbg>(vec), -inv_pi, -inv_pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbba>(vec), -inv_pi, -inv_pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbbr>(vec), -inv_pi, -inv_pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbbb>(vec), -inv_pi, -inv_pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbgg>(vec), -inv_pi, -inv_pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbga>(vec), -inv_pi, -inv_pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbgr>(vec), -inv_pi, -inv_pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbgb>(vec), -inv_pi, -inv_pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbrg>(vec), -inv_pi, -inv_pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbra>(vec), -inv_pi, -inv_pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbrr>(vec), -inv_pi, -inv_pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bbrb>(vec), -inv_pi, -inv_pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgag>(vec), -inv_pi, -.66669667f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgaa>(vec), -inv_pi, -.66669667f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgar>(vec), -inv_pi, -.66669667f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgab>(vec), -inv_pi, -.66669667f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgbg>(vec), -inv_pi, -.66669667f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgba>(vec), -inv_pi, -.66669667f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgbr>(vec), -inv_pi, -.66669667f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgbb>(vec), -inv_pi, -.66669667f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bggg>(vec), -inv_pi, -.66669667f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgga>(vec), -inv_pi, -.66669667f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bggr>(vec), -inv_pi, -.66669667f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bggb>(vec), -inv_pi, -.66669667f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgrg>(vec), -inv_pi, -.66669667f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgra>(vec), -inv_pi, -.66669667f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgrr>(vec), -inv_pi, -.66669667f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bgrb>(vec), -inv_pi, -.66669667f, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brag>(vec), -inv_pi, pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::braa>(vec), -inv_pi, pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brar>(vec), -inv_pi, pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brab>(vec), -inv_pi, pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brbg>(vec), -inv_pi, pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brba>(vec), -inv_pi, pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brbr>(vec), -inv_pi, pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brbb>(vec), -inv_pi, pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brgg>(vec), -inv_pi, pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brga>(vec), -inv_pi, pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brgr>(vec), -inv_pi, pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brgb>(vec), -inv_pi, pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brrg>(vec), -inv_pi, pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brra>(vec), -inv_pi, pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brrr>(vec), -inv_pi, pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::brrb>(vec), -inv_pi, pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::baag>(vec), -inv_pi, 4e7f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::baaa>(vec), -inv_pi, 4e7f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::baar>(vec), -inv_pi, 4e7f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::baab>(vec), -inv_pi, 4e7f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::babg>(vec), -inv_pi, 4e7f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::baba>(vec), -inv_pi, 4e7f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::babr>(vec), -inv_pi, 4e7f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::babb>(vec), -inv_pi, 4e7f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bagg>(vec), -inv_pi, 4e7f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::baga>(vec), -inv_pi, 4e7f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bagr>(vec), -inv_pi, 4e7f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bagb>(vec), -inv_pi, 4e7f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::barg>(vec), -inv_pi, 4e7f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::bara>(vec), -inv_pi, 4e7f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::barr>(vec), -inv_pi, 4e7f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::barb>(vec), -inv_pi, 4e7f, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbag>(vec), -.66669667f, -inv_pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbaa>(vec), -.66669667f, -inv_pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbar>(vec), -.66669667f, -inv_pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbab>(vec), -.66669667f, -inv_pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbbg>(vec), -.66669667f, -inv_pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbba>(vec), -.66669667f, -inv_pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbbr>(vec), -.66669667f, -inv_pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbbb>(vec), -.66669667f, -inv_pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbgg>(vec), -.66669667f, -inv_pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbga>(vec), -.66669667f, -inv_pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbgr>(vec), -.66669667f, -inv_pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbgb>(vec), -.66669667f, -inv_pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbrg>(vec), -.66669667f, -inv_pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbra>(vec), -.66669667f, -inv_pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbrr>(vec), -.66669667f, -inv_pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gbrb>(vec), -.66669667f, -inv_pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggag>(vec), -.66669667f, -.66669667f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggaa>(vec), -.66669667f, -.66669667f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggar>(vec), -.66669667f, -.66669667f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggab>(vec), -.66669667f, -.66669667f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggbg>(vec), -.66669667f, -.66669667f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggba>(vec), -.66669667f, -.66669667f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggbr>(vec), -.66669667f, -.66669667f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggbb>(vec), -.66669667f, -.66669667f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gggg>(vec), -.66669667f, -.66669667f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggga>(vec), -.66669667f, -.66669667f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gggr>(vec), -.66669667f, -.66669667f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gggb>(vec), -.66669667f, -.66669667f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggrg>(vec), -.66669667f, -.66669667f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggra>(vec), -.66669667f, -.66669667f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggrr>(vec), -.66669667f, -.66669667f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::ggrb>(vec), -.66669667f, -.66669667f, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grag>(vec), -.66669667f, pi, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::graa>(vec), -.66669667f, pi, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grar>(vec), -.66669667f, pi, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grab>(vec), -.66669667f, pi, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grbg>(vec), -.66669667f, pi, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grba>(vec), -.66669667f, pi, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grbr>(vec), -.66669667f, pi, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grbb>(vec), -.66669667f, pi, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grgg>(vec), -.66669667f, pi, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grga>(vec), -.66669667f, pi, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grgr>(vec), -.66669667f, pi, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grgb>(vec), -.66669667f, pi, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grrg>(vec), -.66669667f, pi, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grra>(vec), -.66669667f, pi, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grrr>(vec), -.66669667f, pi, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::grrb>(vec), -.66669667f, pi, pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gaag>(vec), -.66669667f, 4e7f, 4e7f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gaaa>(vec), -.66669667f, 4e7f, 4e7f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gaar>(vec), -.66669667f, 4e7f, 4e7f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gaab>(vec), -.66669667f, 4e7f, 4e7f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gabg>(vec), -.66669667f, 4e7f, -inv_pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gaba>(vec), -.66669667f, 4e7f, -inv_pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gabr>(vec), -.66669667f, 4e7f, -inv_pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gabb>(vec), -.66669667f, 4e7f, -inv_pi, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gagg>(vec), -.66669667f, 4e7f, -.66669667f, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gaga>(vec), -.66669667f, 4e7f, -.66669667f, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gagr>(vec), -.66669667f, 4e7f, -.66669667f, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gagb>(vec), -.66669667f, 4e7f, -.66669667f, -inv_pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::garg>(vec), -.66669667f, 4e7f, pi, -.66669667f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::gara>(vec), -.66669667f, 4e7f, pi, 4e7f);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::garr>(vec), -.66669667f, 4e7f, pi, pi);
  FSFV_CHECK_COMPONENTS(shuffle<Shuf4::garb>(vec), -.66669667f, 4e7f, pi, -inv_pi);
}

/*
TEST_CASE("ShufflesVec3RGBA", "[shuffle]")
{
  constexpr float pi{3.141592653f};
  constexpr float inv_pi{1.0f/pi};

  LVec3 vec(pi, -.66669667f, -inv_pi);

  {
    LVec3 shuffled = shuffle<Shuf3::rbg>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, -inv_pi, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, -inv_pi, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rbr>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, -inv_pi, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, -inv_pi, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rbb>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, -inv_pi, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, -inv_pi, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rgg>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, -.66669667f, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, -.66669667f, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rgr>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, -.66669667f, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, -.66669667f, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rgb>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, -.66669667f, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, -.66669667f, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rrg>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, pi, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, pi, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rrr>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, pi, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, pi, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::rrb>(vec);
    FSFV_CHECK_COMP3(shuffled, pi, pi, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), pi, pi, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::bbg>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, -inv_pi, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, -inv_pi, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::bbr>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, -inv_pi, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, -inv_pi, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::bbb>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, -inv_pi, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, -inv_pi, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::bgg>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, -.66669667f, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, -.66669667f, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::bgr>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, -.66669667f, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, -.66669667f, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::bgb>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, -.66669667f, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, -.66669667f, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::brg>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, pi, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, pi, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::brr>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, pi, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, pi, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::brb>(vec);
    FSFV_CHECK_COMP3(shuffled, -inv_pi, pi, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -inv_pi, pi, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::gbg>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, -inv_pi, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, -inv_pi, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::gbr>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, -inv_pi, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, -inv_pi, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::gbb>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, -inv_pi, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, -inv_pi, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::ggg>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, -.66669667f, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, -.66669667f, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::ggr>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, -.66669667f, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, -.66669667f, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::ggb>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, -.66669667f, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, -.66669667f, -inv_pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::grg>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, pi, -.66669667f);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, pi, -.66669667f, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::grr>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, pi, pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, pi, pi, 0);
  }
  {
    LVec3 shuffled = shuffle<Shuf3::grb>(vec);
    FSFV_CHECK_COMP3(shuffled, -.66669667f, pi, -inv_pi);
    FSFV_CHECK_COMPONENTS(Vec4(shuffled), -.66669667f, pi, -inv_pi, 0);
  }
}
*/
// clang-format on
#endif
