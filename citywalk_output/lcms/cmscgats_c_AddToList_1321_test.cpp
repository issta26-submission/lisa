// High-quality C++11 unit test suite for the focal function AddToList
// This test suite is self-contained and does not rely on Google Test.
// It provides minimal fakes/mocks for required dependencies and verifies
// key behaviors of AddToList in various scenarios.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain-knowledge oriented notes:
// - We implement minimal versions of the types/functions used by AddToList.
// - We focus on true/false branches, edge cases, and memory management.
// - We do not expose private/internal details beyond what's required for tests.

typedef int cmsBool; // mimic cmsBool in original code
typedef int WRITEMODE; // alias for readability; actual enum values are not critical here

// Candidate keyword: KEYVALUE structure dependencies
struct KEYVALUE {
    char* Keyword;
    char* Subkey;
    KEYVALUE* Next;
    KEYVALUE* NextSubkey;
    char* Value;
    WRITEMODE WriteAs;
};

// Candidate keyword: cmsIT8 placeholder (used only as opaque context in tests)
struct cmsIT8 {
    int dummy;
};

// Global buffer to capture SynError messages for assertions in tests
static std::vector<std::string> g_LastSynErrors;

// Forward declarations of internal dependencies used by AddToList
static cmsBool SynError(cmsIT8* it8, const char* Txt, ...);
static void* AllocChunk(cmsIT8* it8, size_t size);
static char* AllocString(cmsIT8* it8, const char* str);
static cmsBool IsAvailableOnList(KEYVALUE* head, const char* Key, const char* Subkey, KEYVALUE** LastPtr);
static int cmsstrcasecmp(const char* a, const char* b);

// Minimal implementations to support AddToList behavior

static cmsBool SynError(cmsIT8* it8, const char* Txt, ...)
{
    char buffer[512];
    va_list ap;
    va_start(ap, Txt);
    vsnprintf(buffer, sizeof(buffer), Txt, ap);
    va_end(ap);
    g_LastSynErrors.emplace_back(buffer);
    return 1; // simulate error reporting; original returns cmsBool
}

static void* AllocChunk(cmsIT8* it8, size_t size)
{
    (void)it8; // unused in tests
    return std::malloc(size);
}

static char* AllocString(cmsIT8* it8, const char* str)
{
    if (str == NULL) return NULL;
    size_t len = std::strlen(str) + 1;
    char* dup = static_cast<char*>(std::malloc(len));
    if (dup) std::strcpy(dup, str);
    (void)it8;
    return dup;
}

static cmsBool IsAvailableOnList(KEYVALUE* head, const char* Key, const char* Subkey, KEYVALUE** LastPtr)
{
    KEYVALUE* cur = head;
    while (cur != NULL) {
        bool keymatch = (cur->Keyword && Key && std::strcmp(cur->Keyword, Key) == 0);
        bool submatch = true;
        if (cur->Subkey == NULL && Subkey == NULL) submatch = true;
        else if (cur->Subkey != NULL && Subkey != NULL) submatch = (std::strcmp(cur->Subkey, Subkey) == 0);
        else submatch = false;

        if (keymatch && submatch) {
            if (LastPtr) *LastPtr = cur;
            return 1;
        }
        cur = cur->Next;
    }
    if (LastPtr) *LastPtr = NULL;
    return 0;
}

static int cmsstrcasecmp(const char* a, const char* b)
{
    if (a == NULL || b == NULL) return (a == b) ? 0 : (a == NULL ? -1 : 1);
    while (*a && *b) {
        unsigned char ca = static_cast<unsigned char>(std::tolower(*a));
        unsigned char cb = static_cast<unsigned char>(std::tolower(*b));
        if (ca != cb) return (int)ca - (int)cb;
        ++a; ++b;
    }
    return *a - *b;
}

// The focal method under test (as provided, adapted to C++ in this single TU)
static KEYVALUE* AddToList(cmsIT8* it8, KEYVALUE** Head, const char *Key, const char *Subkey, const char* xValue, WRITEMODE WriteAs)
{
    KEYVALUE* p;
    KEYVALUE* last;

    // Check if property is already in list
    if (IsAvailableOnList(*Head, Key, Subkey, &p)) {
        // This may work for editing properties
        if (cmsstrcasecmp(Key, "NUMBER_OF_FIELDS") == 0 ||
            cmsstrcasecmp(Key, "NUMBER_OF_SETS") == 0) {
            SynError(it8, "duplicate key <%s>", Key);
            return NULL;
        }
    }
    else {
        last = p;
        // Allocate the container
        p = (KEYVALUE*) AllocChunk(it8, sizeof(KEYVALUE));
        if (p == NULL)
        {
            SynError(it8, "AddToList: out of memory");
            return NULL;
        }
        // Store name and value
        p->Keyword = AllocString(it8, Key);
        p->Subkey = (Subkey == NULL) ? NULL : AllocString(it8, Subkey);
        // Keep the container in our list
        if (*Head == NULL) {
            *Head = p;
        }
        else
        {
            if (Subkey != NULL && last != NULL) {
                last->NextSubkey = p;
                // If Subkey is not null, then last is the last property with the same key,
                // but not necessarily is the last property in the list, so we need to move
                // to the actual list end
                while (last->Next != NULL)
                         last = last->Next;
            }
            if (last != NULL) last->Next = p;
        }
        p->Next    = NULL;
        p->NextSubkey = NULL;
    }
    p->WriteAs = WriteAs;
    if (xValue != NULL) {
        p->Value   = AllocString(it8, xValue);
    }
    else {
        p->Value   = NULL;
    }
    return p;
}

// Helper to clean up allocated list nodes
static void FreeList(KEYVALUE* head)
{
    while (head) {
        KEYVALUE* next = head->Next;
        free(head->Keyword);
        free(head->Subkey);
        free(head->Value);
        free(head);
        head = next;
    }
}

// Static test scaffolding
struct TestResult {
    std::string name;
    bool pass;
    std::string reason;
};

static std::vector<TestResult> g_TestResults;

static void recordResult(const std::string& name, bool pass, const std::string& reason = "")
{
    g_TestResults.push_back({name, pass, reason});
}

// Test 1: Basic insertion into an empty list
static void test_AddToList_basic()
{
    cmsIT8 it8;
    KEYVALUE* Head = NULL;

    KEYVALUE* res = AddToList(&it8, &Head, "TestKey", NULL, "val", 0);

    bool ok = (res != NULL) &&
              (Head != NULL) &&
              (Head == res) &&
              (Head->Keyword != NULL && std::strcmp(Head->Keyword, "TestKey") == 0) &&
              (Head->Subkey == NULL) &&
              (Head->WriteAs == 0) &&
              (Head->Value != NULL && std::strcmp(Head->Value, "val") == 0) &&
              (Head->Next == NULL) &&
              (Head->NextSubkey == NULL);

    recordResult("test_AddToList_basic", ok,
                 ok ? "" : "Expected a single new node with Key='TestKey', Value='val' and Head==returned node");
    // cleanup
    FreeList(Head);
}

// Test 2: Duplicate key updates existing entry (non-duplicate-reserved-key path)
static void test_AddToList_duplicateUpdate()
{
    cmsIT8 it8;
    KEYVALUE* Head = NULL;

    // First insertion
    KEYVALUE* first = AddToList(&it8, &Head, "DUP_KEY", NULL, "initial", 0);
    bool before ok = (first != NULL) && (Head == first);
    (void)before; // silence unused if compiled differently

    // Now insert again with same key; should return existing node and update its WriteAs/Value
    KEYVALUE* dup = AddToList(&it8, &Head, "DUP_KEY", NULL, "updated", 1);

    bool condition =
        (dup == first) &&
        (Head == first) &&
        (first != NULL) &&
        (first->WriteAs == 1) &&
        (first->Value != NULL && std::strcmp(first->Value, "updated") == 0);

    recordResult("test_AddToList_duplicateUpdate", condition,
                 condition ? "" : "Expected existing node to be updated with new WriteAs/Value");
    FreeList(Head);
}

// Test 3: Reserved-key duplicate triggers SynError
static void test_AddToList_duplicateReservedError()
{
    g_LastSynErrors.clear();

    cmsIT8 it8;
    KEYVALUE* Head = NULL;

    // Create initial entry with reserved key
    KEYVALUE* first = AddToList(&it8, &Head, "NUMBER_OF_FIELDS", NULL, "1", 0);
    if (first == NULL) {
        recordResult("test_AddToList_duplicateReservedError", false, "Failed to create initial reserved-key node");
        return;
    }

    // Attempt to add duplicate reserved key should trigger SynError and return NULL
    KEYVALUE* dup = AddToList(&it8, &Head, "NUMBER_OF_FIELDS", NULL, "2", 0);

    bool errorReported = !g_LastSynErrors.empty();
    bool resultNull = (dup == NULL);
    bool condition = errorReported && resultNull;

    if (!errorReported) {
        recordResult("test_AddToList_duplicateReservedError", false, "Expected SynError to be reported");
    } else if (!resultNull) {
        recordResult("test_AddToList_duplicateReservedError", false, "Expected NULL return on reserved-key duplicate");
    } else {
        // verify message content roughly
        bool contains = false;
        for (const auto& s : g_LastSynErrors) {
            if (s.find("duplicate key <NUMBER_OF_FIELDS>") != std::string::npos) {
                contains = true; break;
            }
        }
        condition = contains;
        recordResult("test_AddToList_duplicateReservedError", condition,
                     condition ? "" : "SynError message did not contain expected text");
    }

    FreeList(Head);
}

// Harness to run all tests and summarize
int main()
{
    // Run tests
    test_AddToList_basic();
    test_AddToList_duplicateUpdate();
    test_AddToList_duplicateReservedError();

    // Summary
    int passCount = 0;
    for (const auto& t : g_TestResults) if (t.pass) ++passCount;

    std::cout << "Test results: " << passCount << "/" << g_TestResults.size() << " passed\n";
    for (const auto& t : g_TestResults) {
        std::cout << (t.pass ? "[PASSED] " : "[FAILED] ") << t.name;
        if (!t.pass && !t.reason.empty()) {
            std::cout << " - " << t.reason;
        }
        std::cout << "\n";
    }

    return (passCount == (int)g_TestResults.size()) ? 0 : 1;
}