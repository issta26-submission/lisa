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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>foo)(?P<second>bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char initial_text[] = "prefix foobar suffix";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    const char rewrite_template[] = "REPL:$first:$second";
    const int rewrite_template_len = (int)(sizeof(rewrite_template) - 1);
    char target_buf[64];
    char text_and_target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text = { NULL, 0 };
    cre2_string_t rewrite = { NULL, 0 };
    cre2_string_t target = { NULL, 0 };
    cre2_string_t text_and_target = { NULL, 0 };
    const char * ng_name = NULL;
    int ng_index = 0;
    int ext_res = 0;
    int gre_res = 0;

    // step 2: Setup
    memset(target_buf, 0, sizeof(target_buf));
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    text.data = initial_text;
    text.length = initial_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = rewrite_template_len;
    target.data = target_buf;
    target.length = 0;
    text_and_target.data = text_and_target_buf;
    text_and_target.length = 0;

    // step 3: Core operations
    ext_res = cre2_extract_re(re, &text, &rewrite, &target);
    gre_res = cre2_global_replace_re(re, &text_and_target, &rewrite);
    cre2_named_groups_iter_next(iter, &ng_name, &ng_index);

    // step 4: Inspect / propagate results
    target.length = target.length + ((ext_res + ng_index) & 15);
    text_and_target.length = text_and_target.length + ((gre_res ^ ext_res) & 31);
    rewrite.length = rewrite.length + ((ng_index + gre_res) & 7);
    (void)ng_name;
    (void)rewrite_template_len;
    (void)pattern_len;

    // step 5: Validate / propagate results (use values to influence state; no branching)
    target.length = target.length + ((int)text.length & 3);
    text_and_target.length = text_and_target.length + ((int)target.length & 7);
    ng_index = ng_index + ((ext_res + gre_res) & 3);
    (void)target_buf;
    (void)text_and_target_buf;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}