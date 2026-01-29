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
//<ID> 2064
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "Alice-12345";
    const int text_len = (int)(sizeof(text) - 1);
    char target_buf[] = "Alice-12345";
    char rewrite_buf[] = "Bob";
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t matches[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int one_line_before = 0;
    int one_line_after = 0;
    int mres = 0;
    int rres = 0;
    long checksum = 0;

    // step 2: Setup (create and tweak options)
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);
    cre2_opt_set_one_line(opt, 1);
    one_line_after = cre2_opt_one_line(opt);

    // step 3: Configure (build a set and compile it, and build a regexp)
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (perform a match and then a replace using the compiled regexp)
    memset(matches, 0, sizeof(matches));
    mres = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    text_and_target.data = target_buf;
    text_and_target.length = (int)(sizeof(target_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    rres = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (create a simple checksum to tie values together)
    checksum = (long)pattern_len + (long)mres + (long)matches[0].length + (long)one_line_before + (long)one_line_after + (long)rres + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}