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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "PRE-1234-suf";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char input_buf[64];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_s;
    cre2_string_t match_s;
    cre2_string_t err_arg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int res_partial = 0;
    int case_flag = 1;
    const int nmatch = 2;

    // step 2: Setup
    memset(input_buf, 0, sizeof(input_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&input_s, 0, sizeof(input_s));
    memset(&match_s, 0, sizeof(match_s));
    memset(&err_arg, 0, sizeof(err_arg));
    memcpy(input_buf, sample_text, (size_t)sample_text_len);
    input_s.data = input_buf;
    input_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    err_arg.data = NULL;
    err_arg.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    enc = cre2_opt_encoding(opt);
    res_partial = cre2_partial_match(pattern, &input_s, &match_s, nmatch);
    cre2_error_arg(re, &err_arg);

    // step 4: Inspect results
    match_s.length = match_s.length + ((res_partial + (int)enc) & 0xF);
    input_s.length = input_s.length + ((match_s.length ^ (int)err_arg.length) & 0x7);
    err_arg.length = err_arg.length + ((match_s.length + res_partial) & 0x3);

    // step 5: Validate / propagate results
    input_buf[0] = (char)((unsigned char)input_buf[0] + ((unsigned)match_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)err_arg.length & 0x7));
    (void)enc;
    (void)case_flag;
    (void)res_partial;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}