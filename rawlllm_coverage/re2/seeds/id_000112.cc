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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    // Set a reasonable memory limit
    cre2_opt_set_max_mem(opt, (int64_t)(16 * 1024 * 1024));

    // Original pattern containing meta-characters
    const char original_pattern[] = "a+(b)?c.*";
    cre2_string_t original_str;
    original_str.data = original_pattern;
    original_str.length = (int)strlen(original_pattern);

    // Prepare buffer for quoted (escaped) pattern and call cre2_quote_meta
    char quoted_buf[256];
    memset(quoted_buf, 0, sizeof(quoted_buf));
    cre2_string_t quoted_str;
    quoted_str.data = quoted_buf;
    quoted_str.length = 0;
    cre2_quote_meta(&quoted_str, &original_str);

    // Ensure quoted_str.length is set (some implementations may write into data)
    if (quoted_str.length == 0) {
        quoted_str.length = (int)strlen(quoted_buf);
    }

    // Compile a regexp using the original pattern; fall back to quoted if needed
    cre2_regexp_t *re = cre2_new(original_str.data, original_str.length, opt);
    if (!re) {
        re = cre2_new(quoted_str.data, quoted_str.length, opt);
    }

    // If compilation failed, clean up and exit successfully for the test harness
    if (!re) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Query number of capturing groups
    int groups = cre2_num_capturing_groups(re);
    (void)groups; // silence unused variable warning in contexts where not used

    // Prepare text and match containers for cre2_consume
    const char text_buf[] = "aaabbbc";
    cre2_string_t text;
    text.data = text_buf;
    text.length = (int)strlen(text_buf);

    char match_buf[128];
    memset(match_buf, 0, sizeof(match_buf));
    cre2_string_t match;
    match.data = match_buf;
    match.length = 0;

    // Use cre2_consume with the original pattern string
    int consume_ret = cre2_consume(original_str.data, &text, &match, 1);
    (void)consume_ret;

    // Retrieve any error string from the compiled regexp (may be NULL or empty)
    const char *err = cre2_error_string(re);
    (void)err;

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}