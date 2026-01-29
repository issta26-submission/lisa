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
//<ID> 1645
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
    const char text[] = "xxfoobaryy foobar zzfoo";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    int match[8];
    size_t nmatched = 0;
    long checksum = 0;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t text_and_target_str;
    int extract_res = 0;
    int replace_res = 0;
    char rewrite_buf[] = "\\1-\\2";
    char target_buf[] = "";
    char text_and_target_buf[] = "xxfoobaryy foobar zzfoo";

    // step 2: Setup (create options, regexp and set)
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    /* cre2_set_new expects a cre2_anchor_t for the second argument.
       Use an explicit cast from 0 to cre2_anchor_t to avoid a
       conversion error while preserving the intended value. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 3: Operate (pattern info and set matching)
    compiled_pat = cre2_pattern(re);
    memset(match, -1, sizeof(match));
    nmatched = cre2_set_match(set, text, text_len, match, (size_t)(sizeof(match) / sizeof(match[0])));

    // step 4: Operate (extract and global replace using the compiled regexp)
    text_str.data = text;
    text_str.length = (int)text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    target_str.data = target_buf;
    target_str.length = 0;
    extract_res = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = (int)(sizeof(text_and_target_buf) - 1);
    replace_res = cre2_global_replace_re(re, &text_and_target_str, &rewrite_str);

    // step 5: Validate (compute a checksum from results to use variables)
    checksum = (long)pattern_len + (long)nmatched + (long)match[0] + (long)match[1] + (long)match[2]
               + (long)extract_res + (long)replace_res + (long)(compiled_pat != NULL)
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)target_str.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}