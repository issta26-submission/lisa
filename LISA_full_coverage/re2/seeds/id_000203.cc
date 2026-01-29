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
//<ID> 203
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
    const char input_text[] = "foo bar foo";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$name";
    const int rewrite_template_len = (int)(sizeof(rewrite_template) - 1);
    char target_buf[64];
    char replace_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text;
    cre2_string_t rewrite;
    cre2_string_t target;
    cre2_string_t text_and_target;
    cre2_string_t rewrite2;
    int extract_res = 0;
    int replace_res = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&target, 0, sizeof(target));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite2, 0, sizeof(rewrite2));
    memset(target_buf, 0, sizeof(target_buf));
    memset(replace_buf, 0, sizeof(replace_buf));
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations
    text.data = input_text;
    text.length = input_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = rewrite_template_len;
    target.data = target_buf;
    target.length = 0;
    extract_res = cre2_extract_re(re, &text, &rewrite, &target);
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    rewrite2.data = replace_buf;
    rewrite2.length = (int)(sizeof(replace_buf) - 1);
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite2);

    // step 4: Validate / propagate results
    target.length = target.length + ((extract_res + replace_res) & 7);
    text.length = text.length + ((replace_res ^ extract_res) & 3);
    text_and_target.length = text_and_target.length + (target.length & 15);
    (void)pattern_len;
    (void)input_text_len;
    (void)rewrite2;
    (void)rewrite;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}