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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-zA-Z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "widget-9876 extra";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char input_buf[128];
    char match_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int never_nl_val = 0;
    int literal_val = 0;
    int match_res = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(input_buf, 0, sizeof(input_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(input_buf, sample_text, (size_t)sample_text_len);
    text_s.data = input_buf;
    text_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_never_nl(opt, 1);
    cre2_opt_set_literal(opt, 0);
    never_nl_val = cre2_opt_never_nl(opt);
    literal_val = cre2_opt_literal(opt);

    // step 4: Create regex
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);

    // step 5: Core operations and validation
    match_res = cre2_easy_match(pattern, pattern_len, text_s.data, text_s.length, &match_s, nmatch);
    match_s.length = match_s.length + ((match_res & 0x7) + (never_nl_val & 0x3) + (literal_val & 0x1));
    text_s.length = text_s.length + ((int)(compiled_pat != NULL ? compiled_pat[0] : 0) & 0xF);
    input_buf[0] = (char)((unsigned char)input_buf[0] + ((unsigned)match_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)text_s.length & 0x7));
    (void)compiled_pat;
    (void)match_res;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}