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
//<ID> 234
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
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char text_buf[128];
    char capture_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t matches[3];
    cre2_string_t error_arg;
    int res_partial = 0;
    const int nmatch = 3;
    cre2_encoding_t enc = CRE2_UNKNOWN;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(capture_buf, 0, sizeof(capture_buf));
    memset(matches, 0, sizeof(matches));
    memset(&text_str, 0, sizeof(text_str));
    memset(&error_arg, 0, sizeof(error_arg));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_str.data = text_buf;
    text_str.length = input_text_len;
    matches[0].data = capture_buf;
    matches[0].length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);
    res_partial = cre2_partial_match(pattern, &text_str, &matches[0], nmatch);
    cre2_error_arg(re, &error_arg);

    // step 4: Inspect results
    matches[0].length = matches[0].length + ((res_partial + (int)enc) & 0xF);
    error_arg.length = error_arg.length + ((matches[0].length ^ (int)res_partial) & 0x7);
    text_str.length = text_str.length + ((error_arg.length & 0x3) + ((int)enc & 0x1));

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_str.length & 0x7));
    capture_buf[0] = (char)((unsigned char)capture_buf[0] + ((unsigned)matches[0].length & 0x7));
    (void)error_arg;
    (void)enc;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}