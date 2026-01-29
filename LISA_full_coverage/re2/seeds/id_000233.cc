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
//<ID> 233
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
    const char input_literal[] = "item-123 extra";
    const int input_len = (int)(sizeof(input_literal) - 1);
    char text_buf[128];
    char match_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_s;
    cre2_string_t matches[3];
    cre2_string_t err_arg;
    int res_partial = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const int nmatch = 3;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&input_s, 0, sizeof(input_s));
    memset(matches, 0, sizeof(matches));
    memset(&err_arg, 0, sizeof(err_arg));
    memcpy(text_buf, input_literal, (size_t)input_len);
    input_s.data = text_buf;
    input_s.length = input_len;
    matches[0].data = match_buf;
    matches[0].length = 0;
    matches[1].data = match_buf + 42;
    matches[1].length = 0;
    matches[2].data = match_buf + 84;
    matches[2].length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    enc = cre2_opt_encoding(opt);
    res_partial = cre2_partial_match(pattern, &input_s, matches, nmatch);
    cre2_error_arg(re, &err_arg);

    // step 4: Inspect results
    matches[0].length = matches[0].length + ((res_partial + (int)enc) & 7);
    matches[1].length = matches[1].length + ((int)err_arg.length & 3);
    input_s.length = input_s.length + ((matches[0].length ^ matches[1].length) & 0xF);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)input_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)matches[0].length & 0x7));
    (void)res_partial;
    (void)enc;
    (void)err_arg;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}