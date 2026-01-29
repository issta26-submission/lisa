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
//<ID> 202
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
    const char input_text[] = "prefix foobar suffix foobar";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_and_target_buf[] = "prefix foobar suffix foobar";
    const int text_and_target_len = (int)(sizeof(text_and_target_buf) - 1);
    const char rewrite_literal[] = "[$1:$2]";
    const int rewrite_literal_len = (int)(sizeof(rewrite_literal) - 1);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;

    cre2_string_t input_str;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;

    int res_extract = 0;
    int res_replace = 0;

    // step 2: Setup
    memset(&input_str, 0, sizeof(input_str));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));

    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    input_str.data = input_text;
    input_str.length = input_text_len;

    text_and_target.data = text_and_target_buf;
    text_and_target.length = text_and_target_len;

    rewrite_str.data = rewrite_literal;
    rewrite_str.length = rewrite_literal_len;

    // extract using the compiled regexp into target_str
    res_extract = cre2_extract_re(re, &input_str, &rewrite_str, &target_str);

    // perform a global replace on the mutable text_and_target buffer
    res_replace = cre2_global_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate / propagate results (use values to influence state; no branching)
    target_str.length = target_str.length + ((res_extract + res_replace) & 0xF);
    text_and_target.length = text_and_target.length + (res_extract & 7);
    cre2_opt_set_case_sensitive(opt, (res_extract ^ res_replace) & 1);
    cre2_opt_set_log_errors(opt, (res_replace >> 1) & 1);
    (void)iter;
    (void)target_str.data;
    (void)text_and_target.data;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}