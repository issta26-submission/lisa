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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and query POSIX syntax flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    int posix_flag = cre2_opt_posix_syntax(opt);
    (void)posix_flag; // silence unused variable warning

    // Prepare an original string containing regex meta-characters and quote it
    const char original_cstr[] = "a+b(c)";
    cre2_string_t original = { original_cstr, (int)(sizeof(original_cstr) - 1) };
    cre2_string_t quoted = { NULL, 0 };
    // Quote meta characters from original into quoted (API may allocate/assign quoted.data)
    (void)cre2_quote_meta(&quoted, &original);

    // Create a regex (use a simple pattern). Use the options created above.
    const char pattern_cstr[] = "a(b)c";
    int pattern_len = (int)(sizeof(pattern_cstr) - 1);
    cre2_regexp_t *rex = cre2_new(pattern_cstr, pattern_len, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare text and a replacement string; attempt a single replace using the compiled regex.
    const char text_buf[] = "zz a(b)c xx a(b)c yy";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };
    const char rewrite_buf[] = "R";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    // Attempt replacement using the compiled regex
    (void)cre2_replace_re(rex, &text_and_target, &rewrite);

    // Demonstrate conversion of cre2_string_t array to cre2_range_t array
    const char text2[] = "hello world";
    cre2_string_t strings[2] = {
        { "hello", 5 },
        { "world", 5 }
    };
    cre2_range_t ranges[2];
    // This populates ranges based on occurrences of strings in text2
    cre2_strings_to_ranges(text2, ranges, strings, 2);

    // Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);

    return 66;
}