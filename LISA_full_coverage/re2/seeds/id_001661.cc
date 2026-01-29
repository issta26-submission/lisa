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
//<ID> 1661
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
    const char source_text[] = "foo bar foobar foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[128];
    const char rewrite[] = "$1-REPL";
    const int rewrite_len = (int)(sizeof(rewrite) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    int easy_rc = 0;
    int add_rc = 0;
    int replace_rc = 0;
    int cs_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cs_flag = cre2_opt_case_sensitive(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_rc = cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    text_and_target_buf[source_text_len] = '\0';

    match_arr[0].data = text_and_target_buf;
    match_arr[0].length = 0;
    match_arr[1].data = NULL;
    match_arr[1].length = 0;
    match_arr[2].data = NULL;
    match_arr[2].length = 0;
    match_arr[3].data = NULL;
    match_arr[3].length = 0;

    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;

    rewrite_str.data = rewrite;
    rewrite_str.length = rewrite_len;

    easy_rc = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, match_arr, 4);
    replace_rc = cre2_replace_re(re, &text_and_target_str, &rewrite_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)cs_flag + (long)easy_rc + (long)replace_rc + (long)add_rc
               + (long)(compiled_pat != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL)
               + (long)text_and_target_str.length + (long)rewrite_str.length + (long)match_arr[0].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}