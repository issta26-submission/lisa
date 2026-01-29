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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([0-9A-Za-z]+)X([a-z]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "ID123Xvalue trailing";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_buf[128];
    char target_buf0[64];
    char target_buf1[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t matches[3];
    cre2_string_t err_arg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int res_partial = 0;
    const int nmatch = 3;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf0, 0, sizeof(target_buf0));
    memset(target_buf1, 0, sizeof(target_buf1));
    memset(matches, 0, sizeof(matches));
    memset(&text_s, 0, sizeof(text_s));
    memset(&err_arg, 0, sizeof(err_arg));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_s.data = text_buf;
    text_s.length = input_text_len;
    matches[0].data = target_buf0;
    matches[0].length = 0;
    matches[1].data = target_buf1;
    matches[1].length = 0;

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    res_partial = cre2_partial_match(pattern, &text_s, matches, nmatch);
    cre2_error_arg(re, &err_arg);

    // step 5: Inspect results / Validate
    matches[0].length = matches[0].length + ((res_partial & 7) + ((int)enc & 3));
    matches[1].length = matches[1].length + ((err_arg.length & 0xF) + ((matches[0].length) & 3));
    text_s.length = text_s.length - ((matches[0].length ^ matches[1].length) & 0xF);
    target_buf0[0] = (char)((unsigned char)target_buf0[0] + ((unsigned)matches[0].length & 0x7));
    target_buf1[0] = (char)((unsigned char)target_buf1[0] + ((unsigned)matches[1].length & 0x7));
    (void)err_arg;
    (void)enc;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}