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
//<ID> 948
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char main_pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int main_pattern_len = (int)(sizeof(main_pattern) - 1);
    const char find_pattern[] = "prefix\\s*([0-9]+)";
    const char set_pat1[] = "12345|67890";
    const char set_pat2[] = "abc|def";
    const char text_buf[] = "prefix 2468 some-text ABC-12345 trailing";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t find_matches[3];
    int set_match_indices[4];
    int add_res1 = 0;
    int add_res2 = 0;
    int compile_res = 0;
    int find_res = 0;
    int dot_nl_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    dot_nl_flag = cre2_opt_dot_nl(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, set_pat1);
    add_res2 = cre2_set_add_simple(set, set_pat2);
    compile_res = cre2_set_compile(set);
    re = cre2_new(main_pattern, main_pattern_len, opt);
    input.data = text_buf;
    input.length = text_len;
    memset(find_matches, 0, sizeof(find_matches));
    memset(set_match_indices, -1, sizeof(set_match_indices));
    find_res = cre2_find_and_consume(find_pattern, &input, find_matches, 2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)dot_nl_flag + (long)compile_res + (long)find_res + (long)add_res1 + (long)add_res2 + (long)find_matches[0].length + (long)set_match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}