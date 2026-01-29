// This is a C++11 test suite for the focal function pcap_next_etherent
// Note: No GoogleTest is used. A lightweight, non-terminating assertion framework is implemented.
// The tests rely on the project's existing C headers providing struct pcap_etherent and the
// function pcap_next_etherent with C linkage.

#include <string.h>
#include <pcap/namedb.h>
#include <nametoaddr.h>
#include <vector>
#include <memory.h>
#include <cstdlib>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>
#include <pcap-types.h>


extern "C" {
    // Include the project headers that define the pcap_etherent struct and the function.
    // The project uses these headers in etherent.c and dependencies.
    #include <pcap/namedb.h>
}

// Function prototype is provided in the included header, but declare explicitly for clarity.
extern "C" struct pcap_etherent* pcap_next_etherent(FILE *fp);

// Simple non-terminating test framework
static int g_failures = 0;

// Assertion helpers (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT_TRUE failed: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_EQ_INT(a,b,msg) do { \
    if ((int)(a) != (int)(b)) { \
        std::cerr << "ASSERT_EQ_INT failed: " << (msg) \
                  << " (got " << (a) << ", expected " << (b) << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_STREQ(a,b,msg) do { \
    if ((a) == nullptr || (b) == nullptr || std::strcmp((a),(b)) != 0) { \
        std::cerr << "ASSERT_STREQ failed: " << (msg) \
                  << " (got '" << (a ? a : "NULL") << "', expected '" << (b ? b : "NULL") << "')" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Utility: create a temporary file with given content (POSIX-compliant approach using tmpnam)
static std::string create_temp_file(const std::string& content) {
    char tmpname[L_tmpnam];
    if (tmpnam(tmpname) == nullptr) {
        std::cerr << "Failed to generate temporary filename" << std::endl;
        std::exit(1);
    }
    std::ofstream ofs(tmpname, std::ios::binary);
    if (!ofs) {
        std::cerr << "Failed to create temporary file: " << tmpname << std::endl;
        std::exit(1);
    }
    ofs.write(content.data(), (std::streamsize)content.size());
    ofs.close();
    return std::string(tmpname);
}

// Test 1: Parse a single valid Ethernet entry line
// Input: "01:23:45:67:89:ab MyDev\n"
// Expectation: ent != NULL; addr[] parsed and name == "MyDev"
static void test_parse_single_valid_line() {
    // Prepare input
    const std::string input = "01:23:45:67:89:ab MyDev\n";
    std::string path = create_temp_file(input);

    FILE* fp = std::fopen(path.c_str(), "r");
    ASSERT_TRUE(fp != nullptr, "Failed to open temp file for test_parse_single_valid_line");

    struct pcap_etherent* ent = pcap_next_etherent(fp);
    ASSERT_TRUE(ent != nullptr, "pcap_next_etherent should return non-NULL for a valid line");

    // Validate parsed address bytes
    ASSERT_EQ_INT(ent->addr[0], 0x01, "addr[0] should be 0x01");
    ASSERT_EQ_INT(ent->addr[1], 0x23, "addr[1] should be 0x23");
    ASSERT_EQ_INT(ent->addr[2], 0x45, "addr[2] should be 0x45");
    ASSERT_EQ_INT(ent->addr[3], 0x67, "addr[3] should be 0x67");
    ASSERT_EQ_INT(ent->addr[4], 0x89, "addr[4] should be 0x89");
    ASSERT_EQ_INT(ent->addr[5], 0xab, "addr[5] should be 0xab");

    // Validate name
    ASSERT_STREQ(ent->name, "MyDev", "Name should be 'MyDev'");

    std::fclose(fp);
    std::remove(path.c_str());
}

// Test 2: Parse two consecutive valid Ethernet entries in one stream
// Input:
// 1) "01:23:45:67:89:ab DevOne\n"
// 2) "02:34:56:78:9a:bc DevTwo\n"
static void test_parse_two_consecutive_valid_lines() {
    const std::string input = "01:23:45:67:89:ab DevOne\n02:34:56:78:9a:bc DevTwo\n";
    std::string path = create_temp_file(input);

    FILE* fp = std::fopen(path.c_str(), "r");
    ASSERT_TRUE(fp != nullptr, "Failed to open temp file for test_parse_two_consecutive_valid_lines");

    struct pcap_etherent* ent1 = pcap_next_etherent(fp);
    ASSERT_TRUE(ent1 != nullptr, "First entry should be parsed");

    ASSERT_EQ_INT(ent1->addr[0], 0x01, "First entry: addr0");
    ASSERT_EQ_INT(ent1->addr[1], 0x23, "First entry: addr1");
    ASSERT_EQ_INT(ent1->addr[2], 0x45, "First entry: addr2");
    ASSERT_EQ_INT(ent1->addr[3], 0x67, "First entry: addr3");
    ASSERT_EQ_INT(ent1->addr[4], 0x89, "First entry: addr4");
    ASSERT_EQ_INT(ent1->addr[5], 0xab, "First entry: addr5");
    ASSERT_STREQ(ent1->name, "DevOne", "First entry name should be 'DevOne'");

    struct pcap_etherent* ent2 = pcap_next_etherent(fp);
    ASSERT_TRUE(ent2 != nullptr, "Second entry should be parsed");

    ASSERT_EQ_INT(ent2->addr[0], 0x02, "Second entry: addr0");
    ASSERT_EQ_INT(ent2->addr[1], 0x34, "Second entry: addr1");
    ASSERT_EQ_INT(ent2->addr[2], 0x56, "Second entry: addr2");
    ASSERT_EQ_INT(ent2->addr[3], 0x78, "Second entry: addr3");
    ASSERT_EQ_INT(ent2->addr[4], 0x9a, "Second entry: addr4");
    ASSERT_EQ_INT(ent2->addr[5], 0xbc, "Second entry: addr5");
    ASSERT_STREQ(ent2->name, "DevTwo", "Second entry name should be 'DevTwo'");

    std::fclose(fp);
    std::remove(path.c_str());
}

// Test 3: Lines starting with non-hex digit are skipped (e.g., comments)
// Input:
// "# a comment line\n"
// "01:02:03:04:05:06 CommentedLine\n"
static void test_skip_non_hex_line_and_parse_next() {
    const std::string input = "# a comment line\n01:02:03:04:05:06 CommentedLine\n";
    std::string path = create_temp_file(input);

    FILE* fp = std::fopen(path.c_str(), "r");
    ASSERT_TRUE(fp != nullptr, "Failed to open temp file for test_skip_non_hex_line_and_parse_next");

    struct pcap_etherent* ent = pcap_next_etherent(fp);
    ASSERT_TRUE(ent != nullptr, "Entry after a comment should be parsed");

    ASSERT_EQ_INT(ent->addr[0], 0x01, "Parsed after comment: addr0");
    ASSERT_EQ_INT(ent->addr[1], 0x02, "Parsed after comment: addr1");
    ASSERT_EQ_INT(ent->addr[2], 0x03, "Parsed after comment: addr2");
    ASSERT_EQ_INT(ent->addr[3], 0x04, "Parsed after comment: addr3");
    ASSERT_EQ_INT(ent->addr[4], 0x05, "Parsed after comment: addr4");
    ASSERT_EQ_INT(ent->addr[5], 0x06, "Parsed after comment: addr5");
    ASSERT_STREQ(ent->name, "CommentedLine", "Name should be 'CommentedLine'");

    std::fclose(fp);
    std::remove(path.c_str());
}

// Test 4: Incomplete line should yield NULL (EOF during address parse)
// Input: "01:23:45:67:89" (incomplete six octets)
static void test_incomplete_line_returns_null() {
    const std::string input = "01:23:45:67:89";
    std::string path = create_temp_file(input);

    FILE* fp = std::fopen(path.c_str(), "r");
    ASSERT_TRUE(fp != nullptr, "Failed to open temp file for test_incomplete_line_returns_null");

    struct pcap_etherent* ent = pcap_next_etherent(fp);
    ASSERT_TRUE(ent == nullptr, "Incomplete line should result in NULL (EOF during parse)");

    std::fclose(fp);
    std::remove(path.c_str());
}

// Test 5: Line with stray non-whitespace after address should be skipped
// Input: "01:02:03:04:05:06XName\n02:03:04:05:06:07 ValidName\n"
static void test_skip_line_due_to_non_whitespace_after_addr() {
    const std::string input = "01:02:03:04:05:06XName\n02:03:04:05:06:07 ValidName\n";
    std::string path = create_temp_file(input);

    FILE* fp = std::fopen(path.c_str(), "r");
    ASSERT_TRUE(fp != nullptr, "Failed to open temp file for test_skip_line_due_to_non_whitespace_after_addr");

    struct pcap_etherent* ent = pcap_next_etherent(fp);
    // The first line should be skipped due to the non-whitespace char after address;
    // The parser should return the second line as the actual entry.
    ASSERT_TRUE(ent != nullptr, "Second valid line should be parsed after skipping invalid first line");

    ASSERT_EQ_INT(ent->addr[0], 0x02, "Second entry: addr0");
    ASSERT_EQ_INT(ent->addr[1], 0x03, "Second entry: addr1");
    ASSERT_EQ_INT(ent->addr[2], 0x04, "Second entry: addr2");
    ASSERT_EQ_INT(ent->addr[3], 0x05, "Second entry: addr3");
    ASSERT_EQ_INT(ent->addr[4], 0x06, "Second entry: addr4");
    ASSERT_EQ_INT(ent->addr[5], 0x07, "Second entry: addr5");
    ASSERT_STREQ(ent->name, "ValidName", "Second entry name should be 'ValidName'");

    std::fclose(fp);
    std::remove(path.c_str());
}

// Main: run all tests and report summary
int main() {
    test_parse_single_valid_line();
    test_parse_two_consecutive_valid_lines();
    test_skip_non_hex_line_and_parse_next();
    test_incomplete_line_returns_null();
    test_skip_line_due_to_non_whitespace_after_addr();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " tests failed" << std::endl;
        return 2;
    }
}