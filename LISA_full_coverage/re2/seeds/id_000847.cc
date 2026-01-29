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
//<ID> 847
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "h(ello)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxhello world";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t consumed_groups[2];
    int consume_res = 0;
    int replace_res = 0;
    int version_rev = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    memset(consumed_groups, 0, sizeof(consumed_groups));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;

    // step 4: Additional operations
    consume_res = cre2_consume(pattern, &text_and_target, consumed_groups, 2);
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);
    version_rev = cre2_version_interface_revision();

    // step 5: Validate
    computed_check = (long)consume_res + (long)replace_res + (long)version_rev + (long)text_and_target.length + (long)consumed_groups[0].length + (long)rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}