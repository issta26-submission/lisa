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
//<ID> 2127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice123 Bob456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "${name}#${2}";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[128];
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t text_and_target = { text_buf, text_len };
    cre2_string_t rewrite = { rewrite_buf, rewrite_len };
    cre2_string_t target = { target_buf, 0 };
    int extract_res = 0;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);

    // step 3: Configure (compile regexp and prepare named-groups iterator)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (perform an extract using pattern and a global replace using compiled regexp)
    memset(target_buf, 0, sizeof(target_buf));
    target.data = target_buf;
    target.length = 0;
    extract_res = cre2_extract(pattern, &text, &rewrite, &target);
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)text.length + (long)rewrite.length + (long)target.length + (long)extract_res + (long)replace_res + (long)(re != NULL) + (long)(opt != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}