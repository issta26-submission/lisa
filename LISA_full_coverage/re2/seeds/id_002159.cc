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
//<ID> 2159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t matches[3];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_ret = 0;
    int replace_ret = 0;
    int add_simple_ret = 0;
    long checksum = 0;

    // step 2: Setup (initialize match containers and options)
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure (compile regexp and create set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_simple_ret = cre2_set_add_simple(set, pattern);

    // step 4: Operate (perform an easy match and a replacement using compiled regexp)
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    const char rewrite_buf[] = "$1:$2";
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)replace_ret + (long)add_simple_ret + (long)matches[0].length + (long)matches[1].length + (long)text_and_target.length + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}