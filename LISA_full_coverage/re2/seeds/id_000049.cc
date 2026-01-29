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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test)(ing)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "this is testing and test only";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t text_str;
    int easy_res = 0;
    int full_res = 0;
    int prog_size = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 2);
    full_res = cre2_full_match(pattern, &text_str, &match, 2);

    // step 5: Validate
    (void)prog_size;
    (void)easy_res;
    (void)full_res;
    (void)match;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}