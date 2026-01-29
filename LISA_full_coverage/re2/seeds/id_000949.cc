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
//<ID> 949
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char set_pattern[] = "foo|bar";
    const char re_pattern[] = "([a-z]+)([0-9]+)";
    const char find_pattern[] = "prefix\\s*([0-9]+)";
    const char text_buf[] = "prefix 777 suffix foo123";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t matches[3];
    int add_res = 0;
    int compile_res = 0;
    int find_res = 0;
    int dotnl_val = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    dotnl_val = cre2_opt_dot_nl(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);
    re = cre2_new(re_pattern, (int)(sizeof(re_pattern) - 1), opt);
    input.data = text_buf;
    input.length = text_len;
    memset(matches, 0, sizeof(matches));
    find_res = cre2_find_and_consume(find_pattern, &input, matches, 2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)dotnl_val + (long)add_res + (long)compile_res + (long)find_res + (long)matches[0].length + (long)matches[1].length + (long)input.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}