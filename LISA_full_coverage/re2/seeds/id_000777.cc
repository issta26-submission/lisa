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
//<ID> 777
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx helloworld yy hello zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    cre2_string_t text;
    const int MAX_NMATCH = 3;
    cre2_string_t matches[MAX_NMATCH];
    int rc_consume = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    text.data = text_buf;
    text.length = text_len;
    memset(matches, 0, sizeof(matches));
    rc_consume = cre2_consume_re(re, &text, matches, MAX_NMATCH);

    // step 5: Validate
    computed_check = rc_consume + pattern_len + (int)matches[0].length + (int)matches[1].length + (int)matches[2].length + (int)(pattern_from_re ? pattern_from_re[0] : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}