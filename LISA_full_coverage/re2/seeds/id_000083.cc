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
//<ID> 83
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
    char text_buffer[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text_buffer) - 1);
    char rewrite_buffer[] = "$1-REWRITTEN";
    const int rewrite_len = (int)(sizeof(rewrite_buffer) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char * re_pattern_str = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int replace_res = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = text_buffer;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buffer;
    rewrite.length = rewrite_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_posix_syntax(opt, 0);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    replace_res = cre2_global_replace(pattern, &text_and_target, &rewrite);

    // step 5: Validate
    re_pattern_str = cre2_pattern(re);
    enc = cre2_opt_encoding(opt);
    (void)re_pattern_str;
    (void)enc;
    (void)replace_res;
    (void)text_and_target;
    (void)rewrite;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}