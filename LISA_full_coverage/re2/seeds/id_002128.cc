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
//<ID> 2128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Bob-42 and Alice-123 and Eve-7";
    int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:#$2";
    int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char extract_target_buf[64];
    memset(extract_target_buf, 0, sizeof(extract_target_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_str = { text_buf, text_len };
    cre2_string_t rewrite_str = { rewrite_buf, rewrite_len };
    cre2_string_t target_str = { extract_target_buf, (int)(sizeof(extract_target_buf) - 1) };
    int extract_result = 0;
    int replace_result = 0;
    long checksum = 0;

    // step 2: Setup (create options and configure)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure (compile a regexp using the options and create named groups iterator)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (use cre2_extract with the literal pattern and then perform a global replace using the compiled regexp)
    extract_result = cre2_extract(pattern, &text_str, &rewrite_str, &target_str);
    replace_result = cre2_global_replace_re(re, &text_str, &rewrite_str);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)extract_result + (long)replace_result + (long)target_str.length + (long)(re != NULL) + (long)(opt != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}