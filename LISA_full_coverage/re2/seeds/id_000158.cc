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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "item123 remaining";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "<$1:$2>";
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_res = 0;
    int err_code = 0;
    int combined = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);

    // step 3: Configure (no external options used; placeholder for real configs)
    (void)pattern_len;

    // step 4: Operate (compile and perform replace)
    re = cre2_new(pattern, pattern_len, NULL);
    err_code = cre2_error_code(re);
    text_and_target.length = text_and_target.length + (err_code & 3);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / propagate results
    combined = replace_res + text_and_target.length + rewrite.length + err_code;
    rewrite.length = rewrite.length + (combined & 7);
    (void)combined;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}