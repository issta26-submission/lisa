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
//<ID> 267
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
    const char sample_text[] = "widget-2025 extra";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char input_buf[64];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_s;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int flag_never_nl = 0;
    int flag_literal = 0;
    int match_res = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(input_buf, 0, sizeof(input_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&input_s, 0, sizeof(input_s));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(input_buf, sample_text, (size_t)sample_text_len);
    input_s.data = input_buf;
    input_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure options (use setters, then retrieve with getters)
    cre2_opt_set_never_nl(opt, 1);
    cre2_opt_set_literal(opt, 0);
    flag_never_nl = cre2_opt_never_nl(opt);
    flag_literal = cre2_opt_literal(opt);

    // step 4: Create regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Core operations and validation
    match_res = cre2_easy_match(pattern, pattern_len, input_s.data, input_s.length, &match_s, nmatch);
    compiled_pat = re ? cre2_pattern(re) : NULL;
    match_s.length = match_s.length + ((match_res & 0x7) + (flag_never_nl & flag_literal));
    input_s.length = input_s.length + (((int)(compiled_pat ? compiled_pat[0] : 0)) & 0xF);
    input_buf[0] = (char)((unsigned char)input_buf[0] + ((unsigned)match_s.length & 0xF));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)input_s.length & 0xF));
    (void)compiled_pat;
    (void)match_res;
    (void)flag_never_nl;
    (void)flag_literal;

    // step 6: Cleanup
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}