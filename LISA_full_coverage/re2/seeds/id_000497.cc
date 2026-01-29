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
//<ID> 497
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
    const char text_buf[] = "xxfoobar foo bar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char replacement[] = "REPL";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t extract_text;
    cre2_string_t extract_target;
    int nmatch = 4;
    int match_ret = 0;
    int grepl_ret = 0;
    int extract_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&extract_text, 0, sizeof(extract_text));
    memset(&extract_target, 0, sizeof(extract_target));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = replacement;
    rewrite.length = (int)(sizeof(replacement) - 1);
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    grepl_ret = cre2_global_replace(pattern, &text_and_target, &rewrite);

    // step 4: Additional operate - extraction using the (possibly) modified text
    extract_text.data = text_and_target.data;
    extract_text.length = text_and_target.length;
    extract_ret = cre2_extract(pattern, &extract_text, &rewrite, &extract_target);

    // step 5: Validate
    computed_check = match_ret + grepl_ret + extract_ret + (int)match_arr[0].length + extract_target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}