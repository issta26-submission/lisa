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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char work_buf[64] = "prefix foobar suffix";
    const int work_buf_len = input_text_len;
    char rewrite_buf[] = "$2-$1";
    const int rewrite_buf_len = (int)(sizeof(rewrite_buf) - 1);
    char extract_rewrite_buf[] = "$1";
    const int extract_rewrite_buf_len = (int)(sizeof(extract_rewrite_buf) - 1);
    char extract_target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t extract_text;
    cre2_string_t extract_rewrite;
    cre2_string_t extract_target;
    int res_extract = 0;
    int res_replace = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&extract_text, 0, sizeof(extract_text));
    memset(&extract_rewrite, 0, sizeof(extract_rewrite));
    memset(&extract_target, 0, sizeof(extract_target));
    memset(extract_target_buf, 0, sizeof(extract_target_buf));
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    text_and_target.data = work_buf;
    text_and_target.length = work_buf_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_buf_len;
    extract_text.data = input_text;
    extract_text.length = input_text_len;
    extract_rewrite.data = extract_rewrite_buf;
    extract_rewrite.length = extract_rewrite_buf_len;
    extract_target.data = extract_target_buf;
    extract_target.length = 0;

    // step 3: Core operations
    res_extract = cre2_extract_re(re, &extract_text, &extract_rewrite, &extract_target);
    res_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Inspect / propagate results
    extract_target.length = extract_target.length + ((res_extract + res_replace) & 7);
    rewrite.length = rewrite.length + (extract_target.length & 3);
    text_and_target.length = text_and_target.length + ((rewrite.length ^ res_extract) & 5);
    (void)pattern_len;
    (void)input_text_len;
    (void)extract_rewrite_buf_len;

    // step 5: Validate / propagate further
    extract_target.length = extract_target.length + ((int)text_and_target.length & 3);
    rewrite.length = rewrite.length + ((extract_target.length + res_replace) & 1);
    (void)extract_text;
    (void)rewrite;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}