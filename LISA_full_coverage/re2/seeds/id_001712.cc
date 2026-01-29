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
//<ID> 1712
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char extra_pattern[] = "bar";
    const char source_text[] = "xxfoobarbazyy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set_ptr = NULL;
    cre2_string_t match_groups[3];
    int set_match_indexes[8];
    size_t set_matches_count = 0;
    int set_compile_rc = 0;
    int cre2_match_rc = 0;
    int prog_size = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    set_ptr = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_set_add_simple(set_ptr, pattern);
    cre2_set_add_simple(set_ptr, extra_pattern);
    set_compile_rc = cre2_set_compile(set_ptr);

    // step 4: Operate
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);
    set_matches_count = cre2_set_match(set_ptr, text_buf, (size_t)source_text_len, set_match_indexes, (size_t)8);
    match_groups[0].data = NULL; match_groups[0].length = 0;
    match_groups[1].data = NULL; match_groups[1].length = 0;
    match_groups[2].data = NULL; match_groups[2].length = 0;
    cre2_match_rc = cre2_match(re, text_buf, source_text_len, 0, source_text_len, CRE2_UNANCHORED, match_groups, 3);
    prog_size = cre2_program_size(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)set_matches_count + (long)set_compile_rc + (long)cre2_match_rc + (long)prog_size + (long)(match_groups[0].length) + (long)(set_match_indexes[0]);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set_ptr);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}