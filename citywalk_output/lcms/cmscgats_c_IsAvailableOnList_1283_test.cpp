// Automated unit tests for IsAvailableOnList (focal method) without using GTest.
// This test suite is self-contained and can be compiled with C++11.
// The test reproduces the necessary dependencies (KEYVALUE, cmsBool, cmsstrcasecmp, etc.)
// to exercise IsAvailableOnList behavior in isolation.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstring>


// Minimal re-implementation of the dependencies to test the focal function in isolation.

typedef int cmsBool;
#define TRUE 1
#define FALSE 0

struct KEYVALUE {
    const char* Keyword;
    const char* Subkey;
    KEYVALUE* Next;
    KEYVALUE* NextSubkey;
};

// Case-insensitive string comparison (simple cmsstrcasecmp emulation)
int cmsstrcasecmp(const char* a, const char* b)
{
    if (a == nullptr && b == nullptr) return 0;
    if (a == nullptr) return -1;
    if (b == nullptr) return 1;

    while (*a && *b) {
        char ca = *a;
        char cb = *b;
        if (ca >= 'A' && ca <= 'Z') ca = ca - 'A' + 'a';
        if (cb >= 'A' && cb <= 'Z') cb = cb - 'A' + 'a';
        if (ca != cb) return (ca < cb) ? -1 : 1;
        ++a; ++b;
    }
    if (*a == '\0' && *b == '\0') return 0;
    return (*a == '\0') ? -1 : 1;
}

// The focal method under test (reproduced for unit testing in isolation)
cmsBool IsAvailableOnList(KEYVALUE* p, const char* Key, const char* Subkey, KEYVALUE** LastPtr)
{
    {
        if (LastPtr) *LastPtr = p;
        for (;  p != NULL; p = p->Next) {
            if (LastPtr) *LastPtr = p;
            if (*Key != '#') { // Comments are ignored
                if (cmsstrcasecmp(Key, p->Keyword) == 0)
                    break;
            }
        }
        if (p == NULL)
            return FALSE;
        if (Subkey == 0)
            return TRUE;
        for (; p != NULL; p = p->NextSubkey) {
            if (p ->Subkey == NULL) continue;
            if (LastPtr) *LastPtr = p;
            if (cmsstrcasecmp(Subkey, p->Subkey) == 0)
                return TRUE;
        }
        return FALSE;
    }
}

// Simple helpers to build test lists
KEYVALUE* mkNode(const char* kw, const char* sub, KEYVALUE* next = nullptr, KEYVALUE* nextSub = nullptr)
{
    KEYVALUE* n = new KEYVALUE;
    n->Keyword = kw;
    n->Subkey = sub;
    n->Next = next;
    n->NextSubkey = nextSub;
    return n;
}

void deleteList(KEYVALUE* head)
{
    // Free simple linked structure with possible NextSubkey sublists
    while (head) {
        KEYVALUE* cur = head;
        head = head->Next;
        // Free NextSubkey chain if present (non-owning in this test, but clean up anyway)
        KEYVALUE* sub = cur->NextSubkey;
        while (sub) {
            KEYVALUE* t = sub->NextSubkey;
            delete sub;
            sub = t;
        }
        delete cur;
    }
}

// Simple non-terminating assertion helper: records and prints result without aborting
struct TestLogger {
    std::string name;
    bool passed;
    std::string message;
    TestLogger(const std::string& n) : name(n), passed(true), message("") {}
    void fail(const std::string& msg) {
        passed = false;
        message = msg;
    }
};

// Test 1: Basic match on Key; Subkey = NULL; LastPtr should point to the matching node; FUNCTION should return TRUE
void test_BasicKeyMatch_LastPtrEqualsMatchingNode() {
    TestLogger log("Test1_BasicKeyMatch_LastPtrEqualsMatchingNode");

    // Build list: head -> second
    KEYVALUE* n1 = mkNode("Alpha", nullptr);
    KEYVALUE* n2 = mkNode("Beta", nullptr);
    n1->Next = n2;

    KEYVALUE* last = nullptr;
    cmsBool res = IsAvailableOnList(n1, "Beta", nullptr, &last);

    bool cond = (res == TRUE) && (last == n2);

    if (!cond) {
        log.fail("Expected TRUE and LastPtr to point to node with Keyword 'Beta'.");
    }

    // Cleanup
    deleteList(n1);
    if (!log.passed) {
        std::cout << log.name << " FAILED: " << log.message << std::endl;
    } else {
        std::cout << log.name << " PASSED." << std::endl;
    }
}

// Test 2: Key does not exist in list; Subkey = NULL; Should return FALSE; LastPtr should end up at the last node
void test_KeyMissing_ReturnsFalse_LastPtrAtLast() {
    TestLogger log("Test2_KeyMissing_ReturnsFalse_LastPtrAtLast");

    KEYVALUE* n1 = mkNode("Alpha", nullptr);
    KEYVALUE* n2 = mkNode("Beta", nullptr);
    n1->Next = n2;

    KEYVALUE* last = nullptr;
    cmsBool res = IsAvailableOnList(n1, "Delta", nullptr, &last);

    bool cond = (res == FALSE) && (last == n2);

    if (!cond) {
        log.fail("Expected FALSE and LastPtr to point to the last node (Beta).");
    }

    deleteList(n1);
    if (!log.passed) {
        std::cout << log.name << " FAILED: " << log.message << std::endl;
    } else {
        std::cout << log.name << " PASSED." << std::endl;
    }
}

// Test 3: Key matches; Subkey chain contains a matching Subkey; Should return TRUE and LastPtr points to matching Subkey node
void test_KeyMatch_WithSubkeyMatch_LastPtrAtSubnode() {
    TestLogger log("Test3_KeyMatch_WithSubkeyMatch_LastPtrAtSubnode");

    // Build list:
    // Node A: Keyword "Beta" Subkey "sc"
    // Node B: NextSubkey with Subkey "subA" (matching)
    KEYVALUE* n1 = mkNode("Alpha", nullptr);
    KEYVALUE* n2 = mkNode("Beta", "sc");
    KEYVALUE* subA = mkNode(nullptr, "subA"); // Subkey node under n2
    n2->NextSubkey = subA;
    n1->Next = n2;

    KEYVALUE* last = nullptr;
    cmsBool res = IsAvailableOnList(n1, "Beta", "subA", &last);

    bool cond = (res == TRUE) && (last == subA);

    if (!cond) {
        log.fail("Expected TRUE when Subkey 'subA' exists under the matched Key 'Beta'; LastPtr should point to subA.");
    }

    deleteList(n1);
    if (!log.passed) {
        std::cout << log.name << " FAILED: " << log.message << std::endl;
    } else {
        std::cout << log.name << " PASSED." << std::endl;
    }
}

// Test 4: Key matches but Subkey not found in NextSubkey chain; Should return FALSE; LastPtr should end at last visited Subkey (last in chain)
void test_KeyMatch_SubkeyNotFound_ReturnsFalse_LastPtrAtLastSubkey() {
    TestLogger log("Test4_KeyMatch_SubkeyNotFound_ReturnsFalse_LastPtrAtLastSubkey");

    // Build list:
    // Node n2: Keyword "Beta" Subkey "sc0"
    // NextSubkey: subkey "notfound" (no match)
    KEYVALUE* head = mkNode("Alpha", nullptr);
    KEYVALUE* n2 = mkNode("Beta", "sc0");
    KEYVALUE* subNot = mkNode(nullptr, "notfound");
    n2->NextSubkey = subNot;
    head->Next = n2;

    KEYVALUE* last = nullptr;
    cmsBool res = IsAvailableOnList(head, "Beta", "subA", &last);

    // Should be FALSE; LastPtr ends at last visited subnode (subNot)
    bool cond = (res == FALSE) && (last == subNot);

    if (!cond) {
        log.fail("Expected FALSE when Subkey not found; LastPtr should point to the last visited Subkey node (notfound).");
    }

    deleteList(head);
    if (!log.passed) {
        std::cout << log.name << " FAILED: " << log.message << std::endl;
    } else {
        std::cout << log.name << " PASSED." << std::endl;
    }
}

// Test 5: Key begins with '#'; Comments are ignored; Should not match any node; Returns FALSE
void test_KeyStartingWithHash_Ignored() {
    TestLogger log("Test5_KeyStartingWithHash_Ignored");

    KEYVALUE* head = mkNode("Alpha", nullptr);
    KEYVALUE* second = mkNode("Beta", nullptr);
    head->Next = second;

    KEYVALUE* last = nullptr;
    cmsBool res = IsAvailableOnList(head, "#Beta", nullptr, &last);

    bool cond = (res == FALSE) && (last == second || last == head);

    if (!cond) {
        log.fail("Expected FALSE when Key starts with '#', i.e., comments ignored; LastPtr should reflect last node touched.");
    }

    deleteList(head);
    if (!log.passed) {
        std::cout << log.name << " FAILED: " << log.message << std::endl;
    } else {
        std::cout << log.name << " PASSED." << std::endl;
    }
}

// Test 6: Subkey search succeeds only when a later subnode has the matching Subkey; ensures traversal through NextSubkey works
void test_SubkeySearchThroughSubnodes() {
    TestLogger log("Test6_SubkeySearchThroughSubnodes");

    // Build list:
    // Node head -> Node mid (Keyword "Beta", Subkey NULL) -> NextSubkey chain
    // Subnode1: Subkey "subX" (non-matching)
    // Subnode2: Subkey "target" (matching)
    KEYVALUE* head = mkNode("Alpha", nullptr);
    KEYVALUE* mid = mkNode("Beta", nullptr);
    KEYVALUE* s1 = mkNode(nullptr, "subX");
    KEYVALUE* s2 = mkNode(nullptr, "target");
    s1->NextSubkey = s2;
    mid->NextSubkey = s1;
    head->Next = mid;

    KEYVALUE* last = nullptr;
    cmsBool res = IsAvailableOnList(head, "Beta", "target", &last);

    bool cond = (res == TRUE) && (last == s2);

    if (!cond) {
        log.fail("Expected TRUE when Subkey 'target' exists in NextSubkey chain; LastPtr should point to subnode with 'target'.");
    }

    deleteList(head);
    if (!log.passed) {
        std::cout << log.name << " FAILED: " << log.message << std::endl;
    } else {
        std::cout << log.name << " PASSED." << std::endl;
    }
}

// Main function to run all tests
int main() {
    test_BasicKeyMatch_LastPtrEqualsMatchingNode();
    test_KeyMissing_ReturnsFalse_LastPtrAtLast();
    test_KeyMatch_WithSubkeyMatch_LastPtrAtSubnode();
    test_KeyMatch_SubkeyNotFound_ReturnsFalse_LastPtrAtLastSubkey();
    test_KeyStartingWithHash_Ignored();
    test_SubkeySearchThroughSubnodes();

    return 0;
}