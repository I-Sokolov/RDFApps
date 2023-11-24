// IDSChecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <pch.h>

#include <parser/_xml.h>

static void Print(_xml::_element* elem, int level)
{
    std::string offset(2 * level, ' ');

    auto& name = elem->getName();

    printf("%s<%s", offset.c_str(), name.c_str());

    for (auto attr : elem->attributes()) {
        auto& n = attr->getName();
        auto& v = attr->getValue();
        printf(" %s='%s'", n.c_str(), v.c_str());
    }

    printf(">\n");

    auto& content = elem->getContent();
    if (!content.empty()) {
        printf("%s %s\n", offset.c_str(), content.c_str());
    }

    for (auto child : elem->children()) {
        Print(child, level + 1);
    }

    printf("%s</%s>\n", offset.c_str(), name.c_str());
}

static void TestTryXML()
{
    _xml::_document doc(0);

    const char* filePath = R"(E:\DevArea\buildingSMART\IDS\Documentation\testcases\ids\fail-a_minimal_ids_can_check_a_minimal_ifc_1_2.ids)";

    doc.load(filePath);

    if (auto root = doc.getRoot()) {
        Print(root, 0);
    }
}


int main()
{
    TestTryXML();
}
