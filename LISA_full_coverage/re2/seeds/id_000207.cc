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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_buf[] = "prefix foobar suffix";
    const int input_len = (int)(sizeof(input_buf) - 1);
    char rewrite_buf[] = "$1_$2";
    int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[64];
    memset(target_buf, 0, sizeof(target_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text;
    cre2_string_t rewrite;
    cre2_string_t target;
    const char * ng_name = NULL;
    int ng_index = 0;
    int extract_res = 0;
    int replace_res = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&target, 0, sizeof(target));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations
    text.data = input_buf;
    text.length = input_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    target.data = target_buf;
    target.length = 0;
    extract_res = cre2_extract_re(re, &text, &rewrite, &target);
    replace_res = cre2_global_replace_re(re, &text, &rewrite);
    cre2_named_groups_iter_next(iter, &ng_name, &ng_index);

    // step 4: Inspect / propagate results
    rewrite.length = rewrite.length + ((extract_res + replace_res) & 3);
    target.length = target.length + ((replace_res ^ ng_index) & 7);
    text.length = text.length - ((ng_index + extract_res) & 3);
    (void)ng_name;

    // step 5: Validate / propagate to options
    cre2_opt_set_longest_match(opt, (extract_res + replace_res) & 1);
    cre2_opt_set_log_errors(opt, (target.length >> 1) & 1);
    (void)pattern_len;
    (void)input_len;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}