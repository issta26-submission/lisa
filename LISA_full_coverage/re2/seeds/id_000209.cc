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
//<ID> 209
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
    const char original_text[] = "prefix foobar suffix";
    const int original_text_len = (int)(sizeof(original_text) - 1);
    char text_buf[128];
    char target_buf[128];
    const char rewrite_literal[] = "$2-$1";
    const int rewrite_len = (int)(sizeof(rewrite_literal) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_s;
    cre2_string_t target_s;
    const char * ng_name = NULL;
    int ng_index = 0;
    int res_replace = 0;
    int res_extract = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memset(&target_s, 0, sizeof(target_s));
    /* initialize regex and iterator */
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    /* prepare strings */
    /* copy original text into mutable buffer for in-place global replace */
    memcpy(text_buf, original_text, (size_t)original_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = original_text_len;
    rewrite_s.data = rewrite_literal;
    rewrite_s.length = rewrite_len;
    target_s.data = target_buf;
    target_s.length = 0;
    (void)opt;
    (void)pattern_len;
    (void)original_text_len;

    // step 3: Core operations
    /* perform a global replace using the compiled regex */
    res_replace = cre2_global_replace_re(re, &text_and_target, &rewrite_s);
    /* attempt to extract using the same regex and rewrite pattern into target */
    res_extract = cre2_extract_re(re, &text_and_target, &rewrite_s, &target_s);
    /* fetch one named group from the regex (if any) */
    cre2_named_groups_iter_next(iter, &ng_name, &ng_index);

    // step 4: Inspect results
    /* propagate integer results into string lengths to influence state */
    target_s.length = target_s.length + ((res_replace + res_extract + ng_index) & 7);
    text_and_target.length = text_and_target.length + ((target_s.length) & 3);
    rewrite_s.length = rewrite_s.length + (res_replace & 1);
    /* touch retrieved name pointer to avoid unused warning */
    (void)ng_name;

    // step 5: Validate / propagate results
    /* further mix values into buffers to reflect dependent state */
    /* adjust first characters within bounds without branching */
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_and_target.length & 0x7));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)target_s.length & 0x7));
    (void)rewrite_s;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}