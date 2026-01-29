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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(b*)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "xxabbbczzy";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_buf[64];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t matches[3];
    cre2_string_t err_arg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int res_partial = 0;
    const int nmatch = 3;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(matches, 0, sizeof(matches));
    memset(&err_arg, 0, sizeof(err_arg));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = input_text_len;
    matches[0].data = match_buf;
    matches[0].length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    res_partial = cre2_partial_match(pattern, &text_and_target, matches, nmatch);
    cre2_error_arg(re, &err_arg);

    // step 4: Inspect results
    matches[0].length = matches[0].length + ((res_partial + (int)enc + err_arg.length) & 0xF);
    text_and_target.length = text_and_target.length - ((matches[0].length) & 0x7);
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)matches[0].length & 0x3));
    (void)err_arg;

    // step 5: Validate / propagate results
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)text_and_target.length & 0x5));
    matches[0].length = matches[0].length ^ ((int)enc & 0x3);

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}