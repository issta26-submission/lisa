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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(abc)(?:def)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix abcdef middle abc suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char rewrite_buf[] = "X";
    char target_buf[128];
    char text_and_target_buf[256];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text;
    cre2_string_t rewrite;
    cre2_string_t target;
    cre2_string_t text_and_target;
    int extract_res = 0;
    int global_replace_res = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&target, 0, sizeof(target));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(target_buf, 0, sizeof(target_buf));
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    /* initialize string containers */
    text.data = input_text;
    text.length = input_text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    /* copy input into a mutable buffer for global replace */
    memcpy(text_and_target_buf, input_text, (size_t)input_text_len);
    text_and_target_buf[input_text_len] = '\0';
    text_and_target.data = text_and_target_buf;
    text_and_target.length = input_text_len;
    target.data = target_buf;
    target.length = 0;
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Operate
    extract_res = cre2_extract_re(re, &text, &rewrite, &target);
    global_replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / propagate results (no branching)
    target.length = target.length + ((extract_res + global_replace_res) & 15);
    text_and_target.length = text_and_target.length + ((target.length ^ extract_res) & 7);
    rewrite.length = rewrite.length + ((global_replace_res >> 1) & 3);
    (void)pattern_len;
    (void)input_text_len;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}