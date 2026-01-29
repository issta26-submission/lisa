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
//<ID> 2122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>[A-Za-z]+) (?P<last>[A-Za-z]+) (\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice Smith 2020";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char text_and_target_buf[] = "Alice Smith 2020";
    const int tat_len = (int)(sizeof(text_and_target_buf) - 1);
    char rewrite_buf[] = "$first-$3";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[128];
    memset(target_buf, 0, sizeof(target_buf));
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t text_and_target = { text_and_target_buf, tat_len };
    cre2_string_t rewrite = { rewrite_buf, rewrite_len };
    cre2_string_t target = { target_buf, 0 };
    int ext_res = 0;
    int gr_res = 0;
    const char * found_name = NULL;
    int found_index = -1;
    long checksum = 0;

    // step 2: Setup (compile regexp and create named-groups iterator)
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations (extract with pattern and perform global replace using compiled regexp)
    ext_res = cre2_extract(pattern, &text, &rewrite, &target);
    gr_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Inspect named groups (single-step iteration)
    cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)ext_res + (long)gr_res + (long)text.length + (long)text_and_target.length + (long)rewrite.length + (long)target.length + (long)(re != NULL) + (long)(iter != NULL) + (long)found_index + (long)(found_name != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}