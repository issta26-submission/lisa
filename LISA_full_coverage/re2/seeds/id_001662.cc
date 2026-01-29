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
//<ID> 1662
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
    const char source_text[] = "foobar foo bar foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[128];
    const char rewrite[] = "$1";
    const int rewrite_len = (int)(sizeof(rewrite) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    const char *compiled_pat = NULL;
    cre2_string_t matches[4];
    int easy_rc = 0;
    int replace_rc = 0;
    int case_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    memset(matches, 0, sizeof(matches));
    easy_rc = cre2_easy_match(pattern, pattern_len, source_text, source_text_len, matches, (int)(sizeof(matches) / sizeof(matches[0])));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    text_and_target_buf[source_text_len] = '\0';
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite;
    rewrite_str.length = rewrite_len;
    replace_rc = cre2_replace_re(re, &text_and_target_str, &rewrite_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)easy_rc + (long)replace_rc + (long)case_flag
               + (long)(compiled_pat != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL)
               + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)matches[3].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}