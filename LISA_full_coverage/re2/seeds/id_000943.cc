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
//<ID> 943
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pat[] = "123|456";
    const char text_buf[] = "prefix 123 middle 456 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    int add_res = 0;
    int find_res = 0;
    int dotnl_before = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    dotnl_before = cre2_opt_dot_nl(opt);
    cre2_opt_set_dot_nl(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pat);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    input.data = text_buf;
    input.length = text_len;
    memset(&match, 0, sizeof(match));
    find_res = cre2_find_and_consume(pattern, &input, &match, 1);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)dotnl_before + (long)add_res + (long)find_res + (long)match.length + (long)input.length + (long)pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}