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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1-$2";
    const int rewrite_template_len = (int)(sizeof(rewrite_template) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_res = 0;
    int err_code = 0;
    int combined = 0;

    // step 2: Setup
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = rewrite_template_len;

    // step 3: Operate - compile regex and perform a replace using the compiled regex
    re = cre2_new(pattern, pattern_len, NULL);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Inspect / propagate results
    err_code = cre2_error_code(re);
    combined = replace_res + err_code + text_and_target.length + rewrite.length + pattern_len;
    text_and_target.length = text_and_target.length + (combined & 7);

    // step 5: Use propagated state in a follow-up replace to exercise internal behavior
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);
    combined = combined + replace_res + err_code;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    (void)combined;
    (void)replace_res;
    (void)err_code;
    (void)text_and_target;
    (void)rewrite;
    (void)pattern_len;
    return 66;
}