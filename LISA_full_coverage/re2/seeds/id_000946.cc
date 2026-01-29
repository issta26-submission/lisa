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
//<ID> 946
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char set_pattern1[] = "abc|123";
    const int set_pattern1_len = (int)(sizeof(set_pattern1) - 1);
    const char set_pattern2[] = "789|xyz";
    const int set_pattern2_len = (int)(sizeof(set_pattern2) - 1);
    const char re_pattern[] = "([A-Za-z]+)([0-9]+)";
    const int re_pattern_len = (int)(sizeof(re_pattern) - 1);
    const char consume_pattern[] = "prefix\\s*([0-9]+)";
    const int consume_pattern_len = (int)(sizeof(consume_pattern) - 1);
    const char text_buf[] = "prefix 789 suffix abc123 xyz789";
    const int text_len = (int)(sizeof(text_buf) - 1);

    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    int add_res1 = 0;
    int add_res2 = 0;
    int compile_res = 0;
    int find_consume_res = 0;
    int dotnl_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_dot_nl(opt); // call setter is available but we call getter as required; capture below
    dotnl_flag = cre2_opt_dot_nl(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, set_pattern1);
    add_res2 = cre2_set_add_simple(set, set_pattern2);
    compile_res = cre2_set_compile(set);
    re = cre2_new(re_pattern, re_pattern_len, opt);

    input.data = text_buf;
    input.length = text_len;
    memset(&match, 0, sizeof(match));
    find_consume_res = cre2_find_and_consume(consume_pattern, &input, &match, 1);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)dotnl_flag + (long)compile_res + (long)add_res1 + (long)add_res2 + (long)find_consume_res + (long)input.length + (long)match.length + (long)re_pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}