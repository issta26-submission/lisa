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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "^(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "helloworld";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t captures[3];
    cre2_string_t text_str;
    int easy_res = 0;
    int full_res = 0;
    int prog_size = 0;

    // step 2: Setup (zero structures and create options, compile)
    memset(&match, 0, sizeof(match));
    memset(captures, 0, sizeof(captures));
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Operate (inspect compiled program size, perform easy match and full match)
    prog_size = cre2_program_size(re);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 2);
    full_res = cre2_full_match(pattern, &text_str, captures, 3);

    // step 4: Validate (use returned values in a meaningful way to keep data flow)
    (void)prog_size;
    (void)easy_res;
    (void)full_res;
    (void)match;
    (void)captures;
    (void)text_str;

    // step 5: Cleanup (release compiled regexp and options)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}