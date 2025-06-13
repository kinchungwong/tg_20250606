#include <iostream>
#include <string>
#include <functional>
#include "tg/common/project_macros.hpp"
#include "tg/data/specialized/opaque_subindex.hpp"
#include "tg/testcase/ostrm.hpp"

using tg::data::specialized::OpaqueSubindex;

void opaque_subindex_testcase_1(OStrm cout)
{
    cout << "running opaque_subindex_testcase_1()" << std::endl;

    OpaqueSubindex id1;
    OpaqueSubindex id2;
    OpaqueSubindex id3;

    cout << "id1: " << id1.get_hash() << std::endl;
    cout << "id2: " << id2.get_hash() << std::endl;
    cout << "id3: " << id3.get_hash() << std::endl;

    cout << "id1 == id2: " << (id1 == id2) << std::endl;
    cout << "id1 != id2: " << (id1 != id2) << std::endl;

    // Create a new OpaqueSubindex with a specific type and index
    int owner = 42;
    auto id4 = OpaqueSubindex::create(owner, 5);
    cout << "id4: " << id4.get_hash() << std::endl;

    // Check equality with another OpaqueSubindex
    auto id5 = OpaqueSubindex::create(owner, 5);
    cout << "id4 == id5: " << (id4 == id5) << std::endl;
}

void opaque_subindex_testcase_2(OStrm cout)
{
    cout << "running opaque_subindex_testcase_2()" << std::endl;

    using StrPtr = std::shared_ptr<std::string>;
    using StrWeak = std::weak_ptr<std::string>;

    StrPtr owner1 = std::make_shared<std::string>("owner1");
    StrPtr owner2 = std::make_shared<std::string>("owner2");

    StrWeak weak1 = owner1;

    auto id1_a = OpaqueSubindex::create(owner1, 0);
    auto id1_b = OpaqueSubindex::create(owner1.get(), 0);
    auto id1_c = OpaqueSubindex::create(*owner1, 0);
    auto id1_d = OpaqueSubindex::create(weak1, 0);
    auto id1_e = OpaqueSubindex::create(weak1.lock(), 0);
    auto id1_f = OpaqueSubindex::create(weak1.lock().get(), 0);
    auto id1_g = OpaqueSubindex::create(*weak1.lock(), 0);

    cout << "id1_a: " << id1_a.get_hash() << std::endl;
    cout << "id1_b: " << id1_b.get_hash() << std::endl;
    cout << "id1_c: " << id1_c.get_hash() << std::endl;
    cout << "id1_d: " << id1_d.get_hash() << std::endl;
    cout << "id1_e: " << id1_e.get_hash() << std::endl; 
    cout << "id1_f: " << id1_f.get_hash() << std::endl;
    cout << "id1_g: " << id1_g.get_hash() << std::endl;
    cout << "id1_a == id1_b: " << (id1_a == id1_b) << std::endl;
    cout << "id1_a == id1_c: " << (id1_a == id1_c) << std::endl;
    cout << "id1_a == id1_d: " << (id1_a == id1_d) << std::endl;
    cout << "id1_a == id1_e: " << (id1_a == id1_e) << std::endl;
    cout << "id1_a == id1_f: " << (id1_a == id1_f) << std::endl;
    cout << "id1_a == id1_g: " << (id1_a == id1_g) << std::endl;
}

void opaque_subindex_testcase()
{
    OStrm cout;
    opaque_subindex_testcase_1(cout);
    opaque_subindex_testcase_2(cout);
}
