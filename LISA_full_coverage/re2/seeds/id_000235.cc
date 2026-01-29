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
//<ID> 235
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
    const char input_text[] = "prefix foob extra";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char input_buf[128];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t inputs[1];
    cre2_string_t match_s;
    cre2_string_t err_arg;
    int res_partial = 0;
    const int nmatch = 2;
    cre2_encoding_t enc = CRE2_UNKNOWN;

    // step 2: Setup
    memset(input_buf, 0, sizeof(input_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(inputs, 0, sizeof(inputs));
    memset(&match_s, 0, sizeof(match_s));
    memset(&err_arg, 0, sizeof(err_arg));
    memcpy(input_buf, input_text, (size_t)input_text_len);
    inputs[0].data = input_buf;
    inputs[0].length = input_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    res_partial = cre2_partial_match(pattern, inputs, &match_s, nmatch);
    cre2_error_arg(re, &err_arg);
    enc = cre2_opt_encoding(opt);

    // step 4: Inspect results
    match_s.length = match_s.length + ((res_partial & 7) + ((int)enc & 3));
    inputs[0].length = inputs[0].length + ((err_arg.length ^ match_s.length) & 0xF);

    // step 5: Validate / propagate results
    input_buf[0] = (char)((unsigned char)input_buf[0] + ((unsigned)inputs[0].length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)match_s.length & 0x7));
    (void)err_arg;
    (void)res_partial;
    (void)enc;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}