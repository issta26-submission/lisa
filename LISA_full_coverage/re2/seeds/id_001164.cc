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
//<ID> 1164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "widget-31415 extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text;
    cre2_string_t rewrite;
    cre2_string_t target;
    int extract_ret = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_literal(opt, 0);
    enc = cre2_opt_encoding(opt);

    // step 3: Compile and prepare buffers
    re = cre2_new(pattern, pattern_len, opt);
    memset(target_buf, 0, sizeof(target_buf));
    text.data = text_buf;
    text.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    target.data = target_buf;
    target.length = (int)(sizeof(target_buf));

    // step 4: Operate (extract using compiled regexp and rewrite)
    extract_ret = cre2_extract_re(re, &text, &rewrite, &target);

    // step 5: Validate / Cleanup
    computed_check = (long)extract_ret + (long)enc + (long)text.length + (long)rewrite.length + (long)target.length + (long)pattern_len;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}