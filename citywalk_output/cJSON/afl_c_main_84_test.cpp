// C++11 test harness for the focal method (main) in afl.c
// Notes:
// - This test suite drives the actual afl.c binary (assumed to be compiled as ./afl_bin).
// - It uses simple system() calls to execute the binary with various argv configurations.
// - It captures stdout by redirecting to temporary files and reads them back for assertions.
// - It covers true/false branches by varying argc, the presence of "yes" for printing,
//   and the content of the input data file (which feeds json via read_file + json + 2).
// - It avoids GTest; tests are run from main() via a tiny test harness.
// - The code assumes a POSIX environment (system(), WEXITSTATUS, WIFEXITED, /tmp usage).

#include <algorithm>
#include <../cJSON.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <sys/types.h>
#include <iostream>
#include <functional>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <string>
#include <stdio.h>



// Path to the compiled AFL/C program under test.
// Ensure afl_bin is built prior to running this test suite.
static const std::string BINARY_PATH = "./afl_bin";

// Generate a unique temporary output file path for capturing stdout/stderr.
static std::string generate_temp_output_path() {
    static int counter = 0;
    ++counter;
    char path[64];
    // Using /tmp for temporary storage (typical on Linux/macOS).
    std::snprintf(path, sizeof(path), "/tmp/afl_test_output_%d.txt", counter);
    return std::string(path);
}

// Run the binary with given argv, capture stdout into stdout_out, and report exit code.
// Returns true if execution yielded an exit code (even if non-zero), false if system() failed.
static bool run_program(const std::vector<std::string>& argv,
                        std::string& stdout_out,
                        int& exit_code)
{
    if (argv.empty()) return false;
    // Build command line: ./afl_bin arg1 arg2 ...
    std::string cmd = argv[0];
    for (size_t i = 1; i < argv.size(); ++i) {
        cmd += " \"";
        cmd += argv[i];
        cmd += "\"";
    }

    // Redirect stdout (and stderr) to a temp file to capture program output.
    std::string tmp_out = generate_temp_output_path();
    cmd += " > " + tmp_out + " 2>&1";

    int ret = system(cmd.c_str());
    if (ret == -1) {
        // system() failed
        return false;
    }

    // Determine exit status
    if (WIFEXITED(ret)) {
        exit_code = WEXITSTATUS(ret);
    } else {
        // Abnormal termination
        exit_code = -1;
    }

    // Read captured output
    std::ifstream fin(tmp_out.c_str(), std::ios::in);
    if (fin) {
        std::ostringstream oss;
        oss << fin.rdbuf();
        stdout_out = oss.str();
        fin.close();
    } else {
        stdout_out.clear();
    }

    // Clean up temporary file
    std::remove(tmp_out.c_str());

    return true;
}

// Create a test data file with specified content
static std::string create_temp_input_file(const std::string& content) {
    static int counter = 0;
    ++counter;
    std::string path = "/tmp/afl_test_input_" + std::to_string(counter) + ".txt";
    std::ofstream fout(path.c_str(), std::ios::binary);
    fout << content;
    fout.close();
    return path;
}

// Simple helper to check if a substring exists in a string
static bool contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

// Magnetic tests: each test returns true on pass, false on fail
static bool test_no_args() {
    // Scenario: argc < 2 (no input file). Should print usage and exit with failure.
    std::vector<std::string> argv = { BINARY_PATH };
    std::string out;
    int exit_code = 0;
    bool ok = run_program(argv, out, exit_code);
    if (!ok) return false;
    // Expect non-zero exit code (failure)
    if (exit_code == 0) {
        std::cerr << "test_no_args: expected non-zero exit, got 0\n";
        return false;
    }
    // stdout may contain usage text; no strict assertion to keep test robust
    return true;
}

static bool test_too_many_args() {
    // Scenario: argc > 3. Should print usage and exit with failure.
    std::string input = create_temp_input_file("{\"a\":1}");
    // Provide extra arguments to trigger argc > 3
    std::vector<std::string> argv = { BINARY_PATH, input, "yes", "extra" };
    std::string out;
    int exit_code = 0;
    bool ok = run_program(argv, out, exit_code);
    if (!ok) return false;
    if (exit_code == 0) {
        std::cerr << "test_too_many_args: expected non-zero exit, got 0\n";
        return false;
    }
    // Cleanup
    std::remove(input.c_str());
    return true;
}

static bool test_valid_input_no_print() {
    // Scenario: Valid input file, argc==2, no "yes" -> should exit success and print nothing.
    // Prepare file content: two leading chars 'bb' and valid JSON after them: {"a":1}
    std::string content = "bb{\"a\":1}";
    std::string input = create_temp_input_file(content);
    std::vector<std::string> argv = { BINARY_PATH, input };
    std::string out;
    int exit_code = 0;
    bool ok = run_program(argv, out, exit_code);
    if (!ok) return false;
    if (exit_code != 0) {
        std::cerr << "test_valid_input_no_print: expected exit 0, got " << exit_code << "\n";
        return false;
    }
    // Expect no printing
    // Trim stdout
    out.erase(out.find_last_not_of(" \n\r\t") + 1);
    std::string trimmed = out;
    // If there is any non-space content, fail
    bool has_content = false;
    for (char c : trimmed) { if (!isspace(static_cast<unsigned char>(c))) { has_content = true; break; } }
    if (has_content) {
        std::cerr << "test_valid_input_no_print: expected no stdout, got: " << out << "\n";
        std::remove(input.c_str());
        return false;
    }
    std::remove(input.c_str());
    return true;
}

static bool test_invalid_json_parse() {
    // Scenario: Invalid JSON after json+2 -> cJSON_Parse returns NULL -> EXIT_FAILURE
    // Content: first two chars, then invalid JSON
    std::string content = "gg{invalid";
    std::string input = create_temp_input_file(content);
    std::vector<std::string> argv = { BINARY_PATH, input };
    std::string out;
    int exit_code = 0;
    bool ok = run_program(argv, out, exit_code);
    if (!ok) return false;
    if (exit_code == 0) {
        std::cerr << "test_invalid_json_parse: expected failure exit, got 0\n";
        std::remove(input.c_str());
        return false;
    }
    std::remove(input.c_str());
    return true;
}

static bool test_print_buffered_yes() {
    // Scenario: Enable printing with "yes" and buffered output (json[0]=='b', json[1]=='f')
    // Content: "bf" + valid JSON: {"a":1}
    std::string content = "bf{\"a\":1}";
    std::string input = create_temp_input_file(content);
    std::vector<std::string> argv = { BINARY_PATH, input, "yes" };
    std::string out;
    int exit_code = 0;
    bool ok = run_program(argv, out, exit_code);
    if (!ok) return false;
    if (exit_code != 0) {
        std::cerr << "test_print_buffered_yes: expected exit 0, got " << exit_code << "\n";
        std::remove(input.c_str());
        return false;
    }
    // Expect printed JSON in stdout
    if (!contains(out, "\"a\"")) {
        std::cerr << "test_print_buffered_yes: stdout did not contain expected JSON keys. Output:\n" << out << "\n";
        std::remove(input.c_str());
        return false;
    }
    std::remove(input.c_str());
    return true;
}

static bool test_print_unbuffered_yes_format() {
    // Scenario: Enable printing with "yes" and unbuffered with format (json[0] != 'b', json[1]=='f')
    // Content: "xf" + valid JSON: {"a":1}
    std::string content = "xf{\"a\":1}";
    std::string input = create_temp_input_file(content);
    std::vector<std::string> argv = { BINARY_PATH, input, "yes" };
    std::string out;
    int exit_code = 0;
    bool ok = run_program(argv, out, exit_code);
    if (!ok) return false;
    if (exit_code != 0) {
        std::cerr << "test_print_unbuffered_yes_format: expected exit 0, got " << exit_code << "\n";
        std::remove(input.c_str());
        return false;
    }
    // Expect printed JSON in stdout
    if (!contains(out, "\"a\"")) {
        std::cerr << "test_print_unbuffered_yes_format: stdout did not contain expected JSON keys. Output:\n" << out << "\n";
        std::remove(input.c_str());
        return false;
    }
    std::remove(input.c_str());
    return true;
}

// Run all tests and report results
int main() {
    struct Test {
        std::string name;
        std::function<bool()> func;
    };

    std::vector<Test> tests = {
        {"test_no_args", test_no_args},
        {"test_too_many_args", test_too_many_args},
        {"test_valid_input_no_print", test_valid_input_no_print},
        {"test_invalid_json_parse", test_invalid_json_parse},
        {"test_print_buffered_yes", test_print_buffered_yes},
        {"test_print_unbuffered_yes_format", test_print_unbuffered_yes_format}
    };

    int passed = 0;
    int total = (int)tests.size();

    std::cout << "Running " << total << " unit tests for afl.c main() ..." << std::endl;

    for (const auto& t : tests) {
        bool ok = t.func();
        std::cout << "[ " << (ok ? "PASS" : "FAIL") << " ] " << t.name << std::endl;
        if (ok) ++passed;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}