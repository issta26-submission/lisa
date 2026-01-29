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
//<ID> 1037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo.*(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "start foo123bar middle foo456bar end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    char min_buf[64];
    char max_buf[64];
    cre2_string_t min_str;
    cre2_string_t max_str;
    int pmr_res = 0;
    int match_indices[8];
    size_t match_count = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "foo.*bar");
    cre2_set_add_simple(set, "nomatch");
    cre2_set_compile(set);

    // step 3: Prepare buffers
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    min_str.data = min_buf;
    min_str.length = 0;
    max_str.data = max_buf;
    max_str.length = 0;
    memset(match_indices, 0, sizeof(match_indices));

    // step 4: Operate - compute possible match range and run set match
    pmr_res = cre2_possible_match_range(re, &min_str, &max_str, 32);
    match_count = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)8);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pmr_res + (long)match_count + (long)min_str.length + (long)max_str.length + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}