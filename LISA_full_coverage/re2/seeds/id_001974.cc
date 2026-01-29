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
//<ID> 1974
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobar foo foo";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_match_ret = 0;
    int named_index = -1;
    int set_add_ret = 0;
    int global_replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // step 3: Core operations
    easy_match_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 3);
    re = cre2_new(pattern, pattern_len, opt);
    named_index = cre2_find_named_capturing_groups(re, "name");
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, "(foo)");
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;
    global_replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)easy_match_ret + (long)named_index + (long)set_add_ret + (long)global_replace_ret + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)text_and_target.length;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}