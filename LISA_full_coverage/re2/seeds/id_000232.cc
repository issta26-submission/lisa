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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(item)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix item-42 suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_buf[128];
    char match_buf0[64];
    char match_buf1[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t matches[2];
    cre2_string_t err_arg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int pm_res = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf0, 0, sizeof(match_buf0));
    memset(match_buf1, 0, sizeof(match_buf1));
    memset(&text_s, 0, sizeof(text_s));
    memset(matches, 0, sizeof(matches));
    memset(&err_arg, 0, sizeof(err_arg));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_s.data = text_buf;
    text_s.length = input_text_len;
    matches[0].data = match_buf0;
    matches[0].length = 0;
    matches[1].data = match_buf1;
    matches[1].length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    enc = cre2_opt_encoding(opt);
    pm_res = cre2_partial_match(pattern, &text_s, &matches[0], nmatch);

    // step 4: Inspect results
    cre2_error_arg(re, &err_arg);
    matches[0].length = matches[0].length + ((pm_res + (int)enc) & 7);
    matches[1].length = matches[1].length + ((matches[0].length + (err_arg.length & 3)) & 3);
    text_s.length = text_s.length + ((matches[0].length ^ matches[1].length) & 0xF);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_s.length & 0x7));
    match_buf0[0] = (char)((unsigned char)match_buf0[0] + ((unsigned)matches[0].length & 0x7));
    (void)enc;
    (void)pm_res;
    (void)err_arg;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}