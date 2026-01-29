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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "zabczzz";
    const int text_len = (int)(sizeof(text) - 1);
    char match_buf0[32];
    char match_buf1[32];
    char consume_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[2];
    cre2_string_t in_str;
    cre2_string_t out_str;
    int match_ret = 0;
    int find_consume_ret = 0;
    int rev = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int validation_sum = 0;

    // step 2: Setup
    memset(match_buf0, 0, sizeof(match_buf0));
    memset(match_buf1, 0, sizeof(match_buf1));
    memset(consume_buf, 0, sizeof(consume_buf));
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    rev = cre2_version_interface_revision();

    // step 3: Configure
    matches[0].data = match_buf0;
    matches[0].length = (int)sizeof(match_buf0);
    matches[1].data = match_buf1;
    matches[1].length = (int)sizeof(match_buf1);
    in_str.data = text;
    in_str.length = text_len;
    out_str.data = consume_buf;
    out_str.length = (int)sizeof(consume_buf);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 2);
    find_consume_ret = cre2_find_and_consume_re(re, &in_str, &out_str, 1);

    // step 5: Validate
    validation_sum = match_ret + find_consume_ret + rev + (int)enc;
    (void)validation_sum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}