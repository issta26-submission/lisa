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
//<ID> 2120
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
    char text_buf[] = "Bob-123 and Alice-456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char txt_tgt_buf[] = "Bob-123 and Alice-456";
    const int txt_tgt_len = (int)(sizeof(txt_tgt_buf) - 1);
    char target_buf[64];
    char rewrite_buf[] = "$1";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t text_and_target = { txt_tgt_buf, txt_tgt_len };
    cre2_string_t rewrite = { rewrite_buf, rewrite_len };
    cre2_string_t target = { target_buf, 0 };
    int extract_res = 0;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup (compile regexp and create named-groups iterator)
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Operate (prepare target buffer and perform extract using pattern)
    memset(target_buf, 0, sizeof(target_buf));
    target.data = target_buf;
    target.length = 0;
    extract_res = cre2_extract(pattern, &text, &rewrite, &target);

    // step 4: Operate (perform a global replace using the compiled regexp)
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)extract_res + (long)replace_res + (long)target.length + (long)text.length + (long)text_and_target.length + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_named_groups_iter_delete(iter);

    // API sequence test completed successfully
    return 66;
}