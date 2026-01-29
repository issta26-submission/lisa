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
//<ID> 763
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
    char initial_text[] = "hello foobar world foo";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    char text_and_target_buf[128];
    char rewrite_buf[] = "REPL";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t matches[3];
    int rc_easy = 0;
    int rc_replace = 0;
    int num_groups = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memcpy(text_and_target_buf, initial_text, (size_t)initial_text_len);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(matches, 0, sizeof(matches));
    rc_easy = cre2_easy_match(pattern, pattern_len, text_and_target_buf, (int)strlen(text_and_target_buf), matches, 3);
    text_and_target.data = text_and_target_buf;
    text_and_target.length = (int)strlen(text_and_target_buf);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    rc_replace = cre2_replace_re(re, &text_and_target, &rewrite);
    num_groups = cre2_num_capturing_groups(re);
    enc = cre2_opt_encoding(opt);

    // step 4: Validate
    computed_check = rc_easy + rc_replace + num_groups + (int)enc + text_and_target.length + rewrite.length + matches[0].length + matches[1].length + matches[2].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}