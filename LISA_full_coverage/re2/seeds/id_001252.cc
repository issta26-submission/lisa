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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo[0-9]+";
    const int set_pattern_len = (int)(sizeof(set_pattern) - 1);
    char text_buf[] = "foo123 bar456 baz789";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    size_t set_match_ret = 0;
    int prog_size = 0;
    int matches[4] = {0};
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    /* cre2_set_new expects a cre2_anchor_t; cast 0 to that type to preserve original logic */
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    set_add_ret = cre2_set_add_simple(set, set_pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    set_match_ret = cre2_set_match(set, text_buf, text_len, matches, 4);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)set_pattern_len + (long)text_len
                     + (long)set_add_ret + (long)set_compile_ret + (long)prog_size
                     + (long)set_match_ret + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}