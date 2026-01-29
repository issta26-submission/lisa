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
//<ID> 2158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-zA-Z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo-123 bar-456 baz-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t matches[3];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    char text_and_target_buf[] = "foo-123 foo-123";
    cre2_string_t text_and_target = { text_and_target_buf, (int)(sizeof(text_and_target_buf) - 1) };
    char rewrite_buf[] = "$1:$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int easy_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Core operations (compile, match, replace)
    re = cre2_new(pattern, pattern_len, opt);
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)replace_ret + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)text_and_target.length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}