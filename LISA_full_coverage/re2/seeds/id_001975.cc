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
//<ID> 1975
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
    const char text[] = "foobar foo bar foo";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_ret = 0;
    int named_idx = 0;
    int add_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_log_errors(opt, 0);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Core operations
    easy_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 3);
    add_ret = cre2_set_add_simple(set, pattern);
    named_idx = cre2_find_named_capturing_groups(re, "name");
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = "[$1]";
    rewrite.length = 4;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)easy_ret + (long)named_idx + (long)replace_ret + (long)add_ret + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}