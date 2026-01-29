// Test suite for the focal method HandleSwitches in tificc.c
// The tests run each scenario in a separate child process to isolate static/global state.
// The test harness uses POSIX fork/pipe to capture the child process stdout, which prints the
// internal state after handling switches.
// This file is designed for C++11 compilation, without GoogleTest. It uses plain asserts
// implemented as a lightweight test framework embedded in main().

#include <cstring>
#include <cstdio>
#include <lcms2_plugin.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>


// Forward declaration of the focal function and global state from tificc.c.
// We assume C linkage for the C file.
extern "C" {
    void HandleSwitches(int argc, char *argv[]);
}

// Global variables used/modified by HandleSwitches (assumed types from the source).
// These declarations must match the actual definitions in tificc.c.
extern int StoreAsAlpha;
extern int BlackWhiteCompensation;
extern int PrecalcMode;
extern double ObserverAdaptationState;
extern int EmbedProfile;
extern int GamutCheck;
extern int Verbose;
extern char* cInpProf;
extern char* cOutProf;
extern int lIsDeviceLink;
extern int lIsCUBE;
extern char* cProofing;
extern int Intent;
extern int ProofingIntent;
extern int IgnoreEmbedded;
extern int PixelDepth;
extern double InkLimit;
extern char* SaveEmbedded;
}

// Helper: run a scenario in a separate child process and capture its stdout.
static std::string run_scenario(const std::vector<std::string>& args) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return "";
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return "";
    }

    if (pid == 0) {
        // Child: redirect stdout to pipe and run the switch handler
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Build argv: program name followed by provided args
        int argc = (int)args.size() + 1;
        std::vector<char*> argv(argc + 1);
        argv[0] = (char*)"prog";
        for (size_t i = 0; i < args.size(); ++i) {
            argv[i + 1] = (char*)args[i].c_str();
        }
        argv[argc] = nullptr;

        // Call the focal function
        HandleSwitches(argc, argv.data());

        // In case HandleSwitches returns (it should not in normal operation), exit.
        _exit(0);
    } else {
        // Parent: read child's stdout from pipe
        close(pipefd[1]);
        std::string output;
        char buffer[4096];
        ssize_t r;
        while ((r = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.append(buffer, buffer + r);
        }
        close(pipefd[0]);

        int status = 0;
        waitpid(pid, &status, 0);
        return output;
    }
}

// Helper: parse key=value lines into a map
static std::map<std::string, std::string> parse_output(const std::string& out) {
    std::map<std::string, std::string> m;
    std::istringstream iss(out);
    std::string line;
    while (std::getline(iss, line)) {
        // Expect lines like: Key=Value
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        m[key] = val;
    }
    return m;
}

// Lightweight assertion helper
static void assert_true(bool cond, const std::string& msg) {
    if (!cond) {
        std::cerr << "ASSERT FAIL: " << msg << std::endl;
        // Abort immediately to avoid cascading false positives
        std::terminate();
    }
}

// Pretty print helper for test results
static void print_test_result(const std::string& name, bool pass) {
    std::cout << (pass ? "[PASS] " : "[FAIL] ") << name << std::endl;
}

// Expected default state (no args)
static std::map<std::string, std::string> expected_default_state() {
    std::map<std::string, std::string> m;
    m["StoreAsAlpha"] = "0";
    m["BlackWhiteCompensation"] = "0";
    m["PrecalcMode"] = "0";
    m["ObserverAdaptationState"] = "0.000000";
    m["EmbedProfile"] = "0";
    m["GamutCheck"] = "0";
    m["Verbose"] = "0";
    m["cInpProf"] = "(null)";
    m["cOutProf"] = "(null)";
    m["lIsDeviceLink"] = "0";
    m["lIsCUBE"] = "0";
    m["cProofing"] = "(null)";
    m["Intent"] = "0";
    m["ProofingIntent"] = "0";
    m["IgnoreEmbedded"] = "0";
    m["PixelDepth"] = "0";
    m["InkLimit"] = "0.000000";
    m["SaveEmbedded"] = "(null)";
    return m;
}

// Expected helper for a state snapshot
static std::map<std::string, std::string> expected_state_for(const std::vector<std::pair<std::string,std::string>>& pairs) {
    auto m = expected_default_state();
    for (const auto& p : pairs) {
        m[p.first] = p.second;
    }
    return m;
}

int main() {
    int total = 0;
    int passed = 0;

    // 0) Test: no arguments -> default state
    {
        total++;
        std::vector<std::string> args; // empty -> argc = 1 (program name only)
        std::string out = run_scenario(args);
        auto parsed = parse_output(out);
        auto expected = expected_default_state();

        // Compare subset of keys; ensure all keys exist
        bool ok = true;
        for (const auto& kv : expected) {
            auto it = parsed.find(kv.first);
            if (it == parsed.end()) { ok = false; break; }
            if (it->second != kv.second) { ok = false; break; }
        }

        print_test_result("no_args_default_state", ok);
        if (ok) passed++; else {
            std::cerr << "Expected default for key 'StoreAsAlpha' = " << expected["StoreAsAlpha"]
                      << ", got " << parsed["StoreAsAlpha"] << std::endl;
        }
    }

    // 1) Test: -a -> StoreAsAlpha becomes 1
    {
        total++;
        std::vector<std::string> args = { "-a" };
        std::string out = run_scenario(args);
        auto parsed = parse_output(out);
        // Expected: StoreAsAlpha=1, others default
        auto expected = expected_default_state();
        expected["StoreAsAlpha"] = "1";
        bool ok = true;
        for (const auto& kv : expected) {
            auto it = parsed.find(kv.first);
            if (it == parsed.end()) { ok = false; break; }
            if (it->second != kv.second) { ok = false; break; }
        }
        print_test_result("alpha_flag_sets_store_as_alpha", ok);
        if (ok) passed++; else {
            std::cerr << "Failed to set StoreAsAlpha with -a" << std::endl;
        }
    }

    // 2) Test: -c 2 -> PrecalcMode = 2
    {
        total++;
        std::vector<std::string> args = { "-c", "2" };
        std::string out = run_scenario(args);
        auto parsed = parse_output(out);
        auto expected = expected_default_state();
        expected["PrecalcMode"] = "2";
        bool ok = true;
        for (const auto& kv : expected) {
            auto it = parsed.find(kv.first);
            if (it == parsed.end()) { ok = false; break; }
            if (it->second != kv.second) { ok = false; break; }
        }
        print_test_result("set_precac_mode_to_2", ok);
        if (ok) passed++; else {
            std::cerr << "Expected PrecalcMode=2; got " << parsed["PrecalcMode"] << std::endl;
        }
    }

    // 3) Test: -i in.icc -> cInpProf = "in.icc"
    {
        total++;
        std::vector<std::string> args = { "-i", "in.icc" };
        std::string out = run_scenario(args);
        auto parsed = parse_output(out);
        auto expected = expected_default_state();
        expected["cInpProf"] = "in.icc";
        bool ok = true;
        for (const auto& kv : expected) {
            auto it = parsed.find(kv.first);
            if (it == parsed.end()) { ok = false; break; }
            if (it->second != kv.second) { ok = false; break; }
        }
        print_test_result("input_profile_set_with_-i", ok);
        if (ok) passed++; else {
            std::cerr << "Expected cInpProf=in.icc; got " << parsed["cInpProf"] << std::endl;
        }
    }

    // 4) Test: multiple options in one go
    // -a -g -v -e -W 16 -k 50 -s embed
    {
        total++;
        std::vector<std::string> args = { "-a", "-g", "-v", "-e", "-W", "16", "-k", "50", "-s", "embed" };
        std::string out = run_scenario(args);
        auto parsed = parse_output(out);
        auto expected = expected_default_state();
        expected["StoreAsAlpha"] = "1";
        expected["GamutCheck"] = "1";
        expected["Verbose"] = "1";
        expected["EmbedProfile"] = "1";
        expected["PixelDepth"] = "16";
        expected["InkLimit"] = "50.000000";
        expected["SaveEmbedded"] = "embed";
        bool ok = true;
        for (const auto& kv : expected) {
            auto it = parsed.find(kv.first);
            if (it == parsed.end()) { ok = false; break; }
            if (it->second != kv.second) { ok = false; break; }
        }
        print_test_result("multiple_options_combined", ok);
        if (ok) passed++; else {
            std::cerr << "Combination test failed; state: " << std::endl;
            for (const auto& kv : parsed) {
                std::cerr << kv.first << "=" << kv.second << std::endl;
            }
        }
    }

    // 5) Test: -l profile.icc -> input/output profiles already specified path is allowed only for input alone.
    // Since tests are isolated per process, we can test -l by itself in a new scenario.
    {
        total++;
        std::vector<std::string> args = { "-l", "profile.icc" };
        std::string out = run_scenario(args);
        auto parsed = parse_output(out);
        auto expected = expected_default_state();
        expected["cInpProf"] = "profile.icc";
        expected["lIsDeviceLink"] = "1";
        expected["lIsCUBE"] = "0";
        bool ok = true;
        for (const auto& kv : expected) {
            auto it = parsed.find(kv.first);
            if (it == parsed.end()) { ok = false; break; }
            if (it->second != kv.second) { ok = false; break; }
        }
        print_test_result("link_input_profile_sets_lIsDeviceLink", ok);
        if (ok) passed++; else {
            std::cerr << "Expected cInpProf=profile.icc and lIsDeviceLink=1; got: "
                      << parsed["cInpProf"] << " and " << parsed["lIsDeviceLink"] << std::endl;
        }
    }

    // 6) Test: -u device.icm -> input profile with device-link and Cube mode
    {
        total++;
        std::vector<std::string> args = { "-u", "device.icm" };
        std::string out = run_scenario(args);
        auto parsed = parse_output(out);
        auto expected = expected_default_state();
        expected["cInpProf"] = "device.icm";
        expected["lIsDeviceLink"] = "1";
        expected["lIsCUBE"] = "1";
        bool ok = true;
        for (const auto& kv : expected) {
            auto it = parsed.find(kv.first);
            if (it == parsed.end()) { ok = false; break; }
            if (it->second != kv.second) { ok = false; break; }
        }
        print_test_result("device_link_and_cube_with_-u", ok);
        if (ok) passed++; else {
            std::cerr << "Expected cInpProf=device.icm, lIsDeviceLink=1, lIsCUBE=1; got: "
                      << parsed["cInpProf"] << ", " << parsed["lIsDeviceLink"]
                      << ", " << parsed["lIsCUBE"] << std::endl;
        }
    }

    // 7) Summary
    std::cout << "Total tests: " << total << ", Passed: " << passed << std::endl;
    return (passed == total) ? 0 : 1;
}