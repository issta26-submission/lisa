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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix foo123 suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_buf[128];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    cre2_string_t err_arg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int res_partial = 0;
    const int nmatch = 2;
    int case_flag = 1;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&match_s, 0, sizeof(match_s));
    memset(&err_arg, 0, sizeof(err_arg));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_s.data = text_buf;
    text_s.length = input_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    err_arg.data = NULL;
    err_arg.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);

    // step 3: Core operations
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);
    res_partial = cre2_partial_match(pattern, &text_s, &match_s, nmatch);
    cre2_error_arg(re, &err_arg);

    // step 4: Inspect results
    match_s.length = match_s.length + ((res_partial + (int)enc) & 7) + (err_arg.length & 3);
    text_s.length = text_s.length + ((match_s.length ^ (res_partial & 0xF)) & 0xF);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_s.length & 0x7));
    text_buf[1] = (char)((unsigned char)text_buf[1] + ((unsigned)match_s.length & 0x7));
    (void)err_arg;
    (void)case_flag;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}