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
//<ID> 1007
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 bar foo456 baz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int set_add_res = 0;
    int replace_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations - compile regexp and prepare set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_res = cre2_set_add_simple(set, pattern);

    // step 4: Core operations - prepare strings and perform global replace using compiled regexp
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = 0;
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)set_add_res + (long)replace_res + (long)text_and_target.length + (long)rewrite.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}