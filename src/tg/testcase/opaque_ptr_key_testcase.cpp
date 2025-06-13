#include <iostream>
#include <string>
#include <functional>
#include "tg/common/project_macros.hpp"
#include "tg/data/specialized/opaque_ptr_key.hpp"
#include "tg/testcase/ostrm.hpp"

using tg::data::specialized::OpaquePtrKey;

void opaque_ptr_key_testcase_1(OStrm cout)
{
    std::string str_value = "apple";
    struct LocalStruct { std::type_index t; } local_struct_value {typeid(LocalStruct)};
    auto uptr = std::make_unique<LocalStruct>(local_struct_value);
    auto sptr = std::make_shared<LocalStruct>(local_struct_value);
    auto wptr = std::weak_ptr<LocalStruct>(sptr);
    const auto& uget = *uptr;
    const auto& sget = *sptr;
    auto k1 = OpaquePtrKey::create(str_value);
    auto k2 = OpaquePtrKey::create(local_struct_value);
    auto k3 = OpaquePtrKey::create(uptr);
    auto k4 = OpaquePtrKey::create(sptr);
    auto k5 = OpaquePtrKey::create(wptr);
    auto k6 = OpaquePtrKey::create(uget);
    auto k7 = OpaquePtrKey::create(sget);
    cout << "Hash of k1: " << k1.get_hash() << std::endl;
    cout << "Hash of k2: " << k2.get_hash() << std::endl;
    cout << "Hash of k3: " << k3.get_hash() << std::endl;
    cout << "Hash of k4: " << k4.get_hash() << std::endl;
    cout << "Hash of k5: " << k5.get_hash() << std::endl;
    cout << "Hash of k6: " << k6.get_hash() << std::endl;
    cout << "Hash of k7: " << k7.get_hash() << std::endl;
    cout << "Is k1 == k2? " << (k1 == k2) << std::endl;
    cout << "Is k1 == k3? " << (k1 == k3) << std::endl;
    cout << "Is k1 == k4? " << (k1 == k4) << std::endl;
    cout << "Is k1 == k5? " << (k1 == k5) << std::endl;
    cout << "Is k2 == k3? " << (k2 == k3) << std::endl;
    cout << "Is k2 == k4? " << (k2 == k4) << std::endl;
    cout << "Is k2 == k5? " << (k2 == k5) << std::endl;
    cout << "Is k3 == k4? " << (k3 == k4) << std::endl;
    cout << "Is k3 == k5? " << (k3 == k5) << std::endl;
    cout << "Is k4 == k5? " << (k4 == k5) << std::endl;
}

void opaque_ptr_key_testcase()
{
    OStrm cout;
    opaque_ptr_key_testcase_1(cout);
}
