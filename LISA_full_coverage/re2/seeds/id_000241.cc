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
//<ID> 241
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
    const char input_text[] = "widget-4242 trailing";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char input_buf[64];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_s;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int ver_age = 0;
    int res_easy = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(input_buf, 0, sizeof(input_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&input_s, 0, sizeof(input_s));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(input_buf, input_text, (size_t)input_text_len);
    input_s.data = input_buf;
    input_s.length = input_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    res_easy = cre2_easy_match(pattern, pattern_len, input_s.data, input_s.length, &match_s, nmatch);
    compiled_pat = cre2_pattern(re);
    ver_age = cre2_version_interface_age();

    // step 4: Inspect results
    match_s.length = match_s.length + (res_easy & 1);
    match_s.length = match_s.length + ((int)ver_age & 0xF);
    input_s.length = input_s.length - ((match_s.length & 0x7) ^ (int)(compiled_pat != NULL ? compiled_pat[0] : 0));

    // step 5: Validate / propagate results
    input_buf[0] = (char)((unsigned char)input_buf[0] + ((unsigned)input_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)match_s.length & 0x7));
    (void)compiled_pat;
    (void)res_easy;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}