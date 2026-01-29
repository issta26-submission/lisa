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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char initial_text[] = "foo bar foo";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    const char rewrite_template[] = "X";
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_res = 0;
    int errcode = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = initial_text;
    text_and_target.length = initial_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    re = cre2_new(pattern, pattern_len, NULL);

    // step 3: Operate
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / propagate results
    errcode = cre2_error_code(re);
    text_and_target.length = text_and_target.length + (replace_res & 7) + (errcode & 3);
    (void)replace_res;
    (void)errcode;
    (void)text_and_target;
    (void)rewrite;
    (void)pattern_len;
    (void)initial_text_len;

    // step 5: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}