#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable logging of errors.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // Even if options creation failed, return success sentinel per specification.
        return 66;
    }
    cre2_opt_set_log_errors(opt, 1);

    // Original pattern that contains metacharacters.
    std::string original = "a+b(c)";
    cre2_string_t orig = { original.c_str(), static_cast<int>(original.size()) };

    // Prepare a buffer for the quoted (escaped) pattern.
    std::vector<char> qbuf(original.size() * 2 + 16);
    cre2_string_t quoted = { qbuf.data(), static_cast<int>(qbuf.size()) };

    // Quote meta characters from original into quoted buffer.
    // cre2_quote_meta returns non-zero on success (treat non-zero as success).
    int quoted_ok = cre2_quote_meta(&quoted, &orig);

    // Choose pattern data and length: prefer quoted if quoting succeeded.
    const char *pattern_data = orig.data;
    int pattern_len = orig.length;
    if (quoted_ok) {
        pattern_data = quoted.data;
        pattern_len = quoted.length;
    }

    // Compile regex using chosen pattern and the options.
    cre2_regexp_t *rex = cre2_new(pattern_data, pattern_len, opt);

    // Prepare text to perform replacement on and the rewrite string.
    std::string text = "test a+b(c) and more a+b(c)";
    cre2_string_t text_and_target = { text.c_str(), static_cast<int>(text.size()) };
    std::string rewrite = "REPL";
    cre2_string_t rewrite_s = { rewrite.c_str(), static_cast<int>(rewrite.size()) };

    // If compilation succeeded, perform a single replace using the compiled regex.
    if (rex) {
        (void)cre2_replace_re(rex, &text_and_target, &rewrite_s);
        cre2_delete(rex);
    }

    // Clean up options.
    cre2_opt_delete(opt);

    return 66;
}