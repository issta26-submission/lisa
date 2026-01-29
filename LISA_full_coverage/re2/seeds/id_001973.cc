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
//<ID> 1973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobar foo baz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_ret = 0;
    int find_ret = 0;
    int add_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations - compile regex and sets
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_ret = cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Matching and named group lookup
    easy_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 3);
    find_ret = cre2_find_named_capturing_groups(re, "g");

    // step 5: Global replace using compiled regexp
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = "$1X";
    rewrite.length = 3;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 6: Validate and Cleanup
    checksum = (long)pattern_len + (long)text_len + (long)easy_ret + (long)find_ret + (long)add_ret + (long)replace_ret + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL) + (long)(text_and_target.length);
    (void)checksum;
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}