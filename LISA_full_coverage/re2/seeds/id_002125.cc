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
//<ID> 2125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>[A-Za-z]+)-(?P<num>\\d+)";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    char text_buf[] = "Bob-42 and Alice-123";
    cre2_string_t text = { text_buf, (int)(sizeof(text_buf) - 1) };
    char rewrite_buf[] = "$first:$num";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    char target_buf[128];
    memset(target_buf, 0, sizeof(target_buf));
    cre2_string_t target = { target_buf, 0 };
    int extract_result = 0;
    int global_replace_result = 0;
    long checksum = 0;

    // step 2: Setup
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure (compile regexp and create named-groups iterator)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (extract using pattern string, then perform global replace using compiled regexp)
    extract_result = cre2_extract(pattern, &text, &rewrite, &target);
    cre2_string_t text_and_target = { text_buf, text.length };
    global_replace_result = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine values to ensure meaningful flow)
    checksum = (long)pattern_len + (long)extract_result + (long)global_replace_result + (long)text_and_target.length + (long)target.length + (long)(re != NULL) + (long)(iter != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}