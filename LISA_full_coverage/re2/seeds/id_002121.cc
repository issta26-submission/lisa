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
//<ID> 2121
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
    char text_buf[] = "Bob42 and Alice99";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "${name}-NUM";
    char target_buf[128];
    memset(target_buf, 0, sizeof(target_buf));
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    cre2_string_t target = { target_buf, (int)(sizeof(target_buf) - 1) };
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int extract_res = 0;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup (perform an extract using the plain pattern API)
    extract_res = cre2_extract(pattern, &text, &rewrite, &target);

    // step 3: Configure (compile the regexp from the same pattern)
    re = cre2_new(pattern, pattern_len, NULL);

    // step 4: Operate (perform a global replace using the compiled regexp)
    replace_res = cre2_global_replace_re(re, &text, &rewrite);

    // step 5: Validate (create a named-groups iterator and combine results)
    iter = cre2_named_groups_iter_new(re);
    checksum = (long)pattern_len + (long)extract_res + (long)replace_res + (long)text.length + (long)rewrite.length + (long)target.length + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}