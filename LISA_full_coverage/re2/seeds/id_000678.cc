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
//<ID> 678
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
    cre2_regexp_t * re = NULL;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    const char * compiled_pat = NULL;
    const char rewrite_src[] = "\\1-\\2";
    int rc_check = 0;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_dot_nl(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    rewrite.data = rewrite_src;
    rewrite.length = (int)(sizeof(rewrite_src) - 1);
    rc_check = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate
    computed_check = rc_check + num_groups + rewrite.length + (int)compiled_pat[0] + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}