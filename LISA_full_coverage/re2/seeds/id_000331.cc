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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(bc)d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxbcdyy";
    const int text_len = (int)(sizeof(text) - 1);
    char workbuf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_arr[3];
    cre2_string_t text_str;
    cre2_string_t consume_match;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int rev = 0;
    int rc_match = 0;
    int rc_consume = 0;
    int computed_sum = 0;

    // step 2: Setup
    memset(workbuf, 0, sizeof(workbuf));
    memset(match_arr, 0, sizeof(match_arr));
    memset(&text_str, 0, sizeof(text_str));
    memset(&consume_match, 0, sizeof(consume_match));
    opt = cre2_opt_new();
    text_str.data = text;
    text_str.length = text_len;

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    rc_match = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 3);
    consume_match.data = workbuf;
    consume_match.length = (int)sizeof(workbuf);
    rc_consume = cre2_find_and_consume_re(re, &text_str, &consume_match, 1);

    // step 5: Validate
    rev = cre2_version_interface_revision();
    computed_sum = rc_match + rc_consume + (int)enc + rev + match_arr[0].length + text_str.length;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)computed_sum;
    return 66;
}