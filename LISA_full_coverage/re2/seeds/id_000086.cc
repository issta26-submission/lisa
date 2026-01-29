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
//<ID> 86
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
    const char initial_text[] = "prefix foobar foo suffix";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    const char rewrite_template[] = "$1-REPLACED";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_count = 0;
    cre2_encoding_t enc;
    const char * re_pat = NULL;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = initial_text;
    text_and_target.length = initial_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_posix_syntax(opt, 0);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    replace_count = cre2_global_replace(pattern, &text_and_target, &rewrite);

    // step 5: Validate
    enc = cre2_opt_encoding(opt);
    re_pat = cre2_pattern(re);
    (void)replace_count;
    (void)enc;
    (void)re_pat;
    (void)text_and_target;
    (void)rewrite;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}