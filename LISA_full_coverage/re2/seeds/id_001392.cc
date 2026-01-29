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
//<ID> 1392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    char set_error[128];
    int set_add_rc = 0;
    int num_groups = 0;
    cre2_string_t text_and_target;
    char textbuf[] = "foo bar foo";
    cre2_string_t rewrite;
    char rewritebuf[] = "X-$1";
    int replace_rc = 0;
    int global_replace_rc = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(set_error, 0, sizeof(set_error));
    memset(&computed_check, 0, sizeof(computed_check));
    opt = cre2_opt_new();
    /* cre2_anchor_t is an enum type; cast 0 to that type to avoid
       implicit conversion errors. This preserves the original logic. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    text_and_target.data = textbuf;
    text_and_target.length = (int)(sizeof(textbuf) - 1);
    rewrite.data = rewritebuf;
    rewrite.length = (int)(sizeof(rewritebuf) - 1);

    // step 4: Core operations
    set_add_rc = cre2_set_add(set, pattern, (size_t)pattern_len, set_error, sizeof(set_error));
    re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        num_groups = cre2_num_capturing_groups(re);
    }
    replace_rc = cre2_replace("foo", &text_and_target, &rewrite);
    global_replace_rc = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)num_groups + (long)set_add_rc
                     + (long)replace_rc + (long)global_replace_rc
                     + (long)(text_and_target.length) + (long)(rewrite.length)
                     + (long)(set != NULL) + (long)(re != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}