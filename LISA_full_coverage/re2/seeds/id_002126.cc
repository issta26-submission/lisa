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
//<ID> 2126
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
    char source_buf[] = "Bob-123 and Alice-456";
    int source_len = (int)(sizeof(source_buf) - 1);
    char extract_rewrite_buf[] = "$1_EXTRACT";
    int extract_rewrite_len = (int)(sizeof(extract_rewrite_buf) - 1);
    char target_buf[64];
    cre2_string_t text = { source_buf, source_len };
    cre2_string_t rewrite = { extract_rewrite_buf, extract_rewrite_len };
    cre2_string_t target = { target_buf, (int)0 };
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    long checksum = 0;
    int extract_ret = 0;
    int replace_ret = 0;
    char replace_text_buf[] = "Bob-123 and Alice-456";
    cre2_string_t text_and_target = { replace_text_buf, (int)(sizeof(replace_text_buf) - 1) };
    char global_rewrite_buf[] = "[$1]";
    cre2_string_t global_rewrite = { global_rewrite_buf, (int)(sizeof(global_rewrite_buf) - 1) };

    // step 2: Setup (initialize buffers and compile regexp)
    memset(target_buf, 0, sizeof(target_buf));
    target.data = target_buf;
    target.length = (int)sizeof(target_buf);
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);

    // step 3: Operate (create named-groups iterator, perform extract, perform global replace using compiled re)
    iter = cre2_named_groups_iter_new((const cre2_regexp_t *)re);
    extract_ret = cre2_extract(pattern, &text, &rewrite, &target);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &global_rewrite);

    // step 4: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)extract_ret + (long)replace_ret + (long)text.length + (long)text_and_target.length + (long)target.length + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}