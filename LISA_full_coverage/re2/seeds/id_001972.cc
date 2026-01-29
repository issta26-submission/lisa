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
//<ID> 1972
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
    const char text[] = "foobar foo foobar";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t matches[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_match_ret = 0;
    int find_named_idx = -1;
    int set_add_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Operate - simple matching and adding to set
    easy_match_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 4);
    set_add_ret = cre2_set_add_simple(set, "(foo|bar)");
    (void)set_add_ret;

    // step 4: Operate - find named capturing group index and perform global replace with compiled regexp
    find_named_idx = cre2_find_named_capturing_groups(re, "g");
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = "$1X";
    rewrite.length = 3;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (compute checksum to use results meaningfully)
    checksum = (long)pattern_len + (long)text_len + (long)easy_match_ret + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)find_named_idx + (long)replace_ret + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}