// Lightweight C++11 unit tests for the focal function print_all_db_locks
// This test suite provides minimal testing (no GTest) and uses a small
// in-process mock of the relevant structures to exercise the behavior.
// Each test captures stdout from the function under test and performs
// non-terminating assertions (via simple boolean checks) to maximize
// execution coverage.

#include <errno.h>
#include <vector>
#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <iostream>


// -------------------------------------------------------------------------------------
// Minimal mock declarations to resemble the dependencies used by print_all_db_locks
// These mirror the tiny subset of the real project's structures used in the focal method.
// -------------------------------------------------------------------------------------

// Forward declaration for the function under test (C linkage)
extern "C" void print_all_db_locks(struct lsm_db *db);
extern "C" void print_db_locks(struct lsm_db *db);

// Lightweight representations of the needed types
struct Database {
    struct lsm_db *pConn; // head of the connection list
};

struct lsm_db {
    Database *pDatabase;  // back-pointer to containing Database structure
    lsm_db *pNext;        // singly linked list of connections
};

// -------------------------------------------------------------------------------------
// Implementation of the focal method (as provided) and a simple print_db_locks
// -------------------------------------------------------------------------------------

// Simple diagnostic stub for per-database lock information
static void print_db_locks(struct lsm_db *db) {
    // In the real project this would print details about locks held by 'db'.
    // For test purposes, print a stable tag with the pointer value to verify order/identity.
    printf("[locks:%p]", (void*)db);
}

// The focal method under test (reproduced here for the isolated unit test environment)
void print_all_db_locks(lsm_db *db){
    lsm_db *p;
    for(p=db->pDatabase->pConn; p; p=p->pNext){
        printf("%s connection %p ", ((p==db)?"*":""), p);
        print_db_locks(p);
    }
}

// -------------------------------------------------------------------------------------
// Very lightweight test framework (non-terminating assertions)
// -------------------------------------------------------------------------------------

#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { failures.push_back(std::string("ASSERT_TRUE: ") + (msg)); } } while(0)
#define ASSERT_EQ(a, b, msg) do { if(!((a) == (b))) { failures.push_back(std::string("ASSERT_EQ: ") + (msg) + " | got: " + std::to_string((long long)(a)) + " expected: " + std::to_string((long long)(b))); } } while(0)

static std::vector<std::string> failures;

// Very small stdout capture helper using a POSIX pipe
class StdoutCapture {
    int saved_stdout_fd;
    int pipe_fd[2];
    bool active;
public:
    StdoutCapture() : saved_stdout_fd(-1), active(false) {
        // Create a pipe to capture stdout
        if (pipe(pipe_fd) == 0) {
            // Save current stdout, then redirect stdout to the write end of the pipe
            saved_stdout_fd = dup(fileno(stdout));
            if (saved_stdout_fd != -1) {
                dup2(pipe_fd[1], fileno(stdout));
                close(pipe_fd[1]);
                active = true;
            }
        }
    }
    ~StdoutCapture() {
        if (active) {
            fflush(stdout);
            // Restore stdout
            dup2(saved_stdout_fd, fileno(stdout));
            close(saved_stdout_fd);
            close(pipe_fd[0]);
        } else {
            // If capture wasn't active, nothing to do
        }
        active = false;
    }
    std::string readCaptured() {
        if (!active) return "";
        // Read all data from the read end of the pipe
        // Note: This read should happen after the code under test has finished writing.
        char buffer[4096];
        std::string out;
        ssize_t n;
        // Read until EOF
        while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            out.append(buffer, (size_t)n);
        }
        return out;
    }
};

// -------------------------------------------------------------------------------------
// Test Scenarios
// Step 2: Unit Test Generation for print_all_db_locks
// We craft a small in-memory graph that mimics the dependencies used by the focal method.
// - Test 1: The db pointer is the head of the pConn chain (star printed on first entry).
// - Test 2: The head is not equal to db, so the first entry is different and star appears on a later entry.
// - Test 3: Single-node chain where the only entry is db (ensures the star is printed once).
// These cover true/false branches of the condition p==db and iterate across multiple nodes.
// -------------------------------------------------------------------------------------

// Helper to build a small graph for tests
static void build_chain_two_nodes(lsm_db *&head, lsm_db *&second, Database *&container) {
    // Build two nodes
    static lsm_db nodeA;
    static lsm_db nodeB;
    static Database dbContainer;

    // Establish container pointers
    nodeA.pDatabase = &dbContainer;
    nodeB.pDatabase = &dbContainer;
    dbContainer.pConn = &nodeB; // head of the chain is nodeB (can be passed as db)

    // Link nodes
    nodeB.pNext = &nodeA;
    nodeA.pNext = NULL;

    // Expose to caller
    head = &nodeB;
    second = &nodeA;
    container = &dbContainer;
}

static void test_print_all_db_locks_head_is_db_true_once() {
    // Scenario: db is the head of the pConn chain
    // Expect: first printed entry corresponds to db with a star "*"
    //         there are two iterations, hence two "[locks:...]" blocks.
    static lsm_db nodeB;
    static lsm_db nodeA;
    static Database dbContainer;

    // Setup chain: db (nodeB) -> nodeA
    nodeB.pDatabase = &dbContainer;
    nodeA.pDatabase = &dbContainer;
    dbContainer.pConn = &nodeB;
    nodeB.pNext = &nodeA;
    nodeA.pNext = NULL;

    // Use db as the focal object
    lsm_db *db = &nodeB;

    // Capture stdout
    StdoutCapture sc;
    print_all_db_locks(db);
    std::string out = sc.readCaptured();

    // Assertions (non-terminating)
    // 1) Output should start with "* connection" indicating p==db on first entry
    ASSERT_TRUE(out.find("* connection") == 0, "Test1: first entry should be star when db is head");
    // 2) There should be exactly two " connection " occurrences (two nodes)
    size_t countConn = 0;
    size_t pos = 0;
    const std::string needle = " connection ";
    while ((pos = out.find(needle, pos)) != std::string::npos) {
        ++countConn;
        pos += needle.length();
    }
    ASSERT_EQ(countConn, 2, "Test1: expected exactly 2 iterations (2 nodes) in output");
    // 3) There should be two "[locks:...]" blocks (one per node)
    size_t countLocks = 0;
    pos = 0;
    while ((pos = out.find("[locks:", pos)) != std::string::npos) {
        ++countLocks;
        pos += 1;
    }
    ASSERT_EQ(countLocks, 2, "Test1: expected exactly 2 lock print blocks");
}

// Helper to run Test 2: head != db, star appears on second entry
static void test_print_all_db_locks_head_not_db_star_on_second() {
    // Scenario: head is A, then B, with db = B
    static lsm_db nodeA;
    static lsm_db nodeB;
    static Database dbContainer;

    // Setup chain: A -> B
    nodeA.pDatabase = &dbContainer;
    nodeB.pDatabase = &dbContainer;
    dbContainer.pConn = &nodeA;
    nodeA.pNext = &nodeB;
    nodeB.pNext = NULL;

    // db points to B
    lsm_db *db = &nodeB;

    // Capture stdout
    StdoutCapture sc;
    print_all_db_locks(db);
    std::string out = sc.readCaptured();

    // Assertions
    // 1) The first token should NOT have a leading "*" (no star on first entry)
    ASSERT_TRUE(out.find("* connection") != 0, "Test2: first entry should not be star when head != db");
    // 2) There should be exactly one star in the entire output (on the second entry)
    size_t starCount = 0;
    size_t pos = 0;
    while ((pos = out.find("* connection ", pos)) != std::string::npos) {
        ++starCount;
        pos += 1;
    }
    ASSERT_EQ(starCount, 1, "Test2: expected exactly 1 star in output when db is second in chain");
    // 3) There should be two "[locks:...]" blocks (two entries)
    size_t countLocks = 0;
    pos = 0;
    while ((pos = out.find("[locks:", pos)) != std::string::npos) {
        ++countLocks;
        pos += 1;
    }
    ASSERT_EQ(countLocks, 2, "Test2: expected exactly 2 lock print blocks");
}

// Helper to run Test 3: single-node chain where db is the only entry
static void test_print_all_db_locks_single_node() {
    // Scenario: single node B which is also db
    static lsm_db nodeB;
    static Database dbContainer;

    nodeB.pDatabase = &dbContainer;
    dbContainer.pConn = &nodeB;
    nodeB.pNext = NULL;

    lsm_db *db = &nodeB;

    // Capture stdout
    StdoutCapture sc;
    print_all_db_locks(db);
    std::string out = sc.readCaptured();

    // Assertions
    // 1) Output should start with "* connection" indicating db is the only entry
    ASSERT_TRUE(out.find("* connection") == 0, "Test3: single-node: first entry should be star since it is db");
    // 2) There should be exactly one "connection" occurrence
    size_t countConn = 0;
    size_t pos = 0;
    const std::string needle = " connection ";
    while ((pos = out.find(needle, pos)) != std::string::npos) {
        ++countConn;
        pos += needle.length();
    }
    ASSERT_EQ(countConn, 1, "Test3: single-node: expected exactly 1 iteration");
    // 3) There should be exactly one "[locks:...]" block
    size_t countLocks = 0;
    pos = 0;
    while ((pos = out.find("[locks:", pos)) != std::string::npos) {
        ++countLocks;
        pos += 1;
    }
    ASSERT_EQ(countLocks, 1, "Test3: single-node: expected exactly 1 lock print block");
}

// -------------------------------------------------------------------------------------
// Test runner (Step 3: Test Case Refinement)
// -------------------------------------------------------------------------------------

static void run_all_tests() {
    test_print_all_db_locks_head_is_db_true_once();
    test_print_all_db_locks_head_not_db_star_on_second();
    test_print_all_db_locks_single_node();
}

// -------------------------------------------------------------------------------------
// Main: Execute tests from the command line (as required by the domain knowledge)
// -------------------------------------------------------------------------------------

int main() {
    // Run all tests; print failures and overall summary
    run_all_tests();

    if (failures.empty()) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed (" << failures.size() << "):\n";
        for (const auto &f : failures) {
            std::cout << " - " << f << "\n";
        }
        return 1;
    }
}