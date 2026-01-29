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
//<ID> 1711
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
    const char source_text[] = "xxfoobarbazyy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    char text_buf[64];
    cre2_string_t pmatch[3];
    int rc_match = 0;
    int match_indices[8];
    size_t set_matches = 0;
    int set_compile_ret = 0;
    int prog_size = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Operate
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);
    memset(pmatch, 0, sizeof(pmatch));
    pmatch[0].data = NULL; pmatch[0].length = 0;
    pmatch[1].data = NULL; pmatch[1].length = 0;
    pmatch[2].data = NULL; pmatch[2].length = 0;
    rc_match = cre2_match(re, text_buf, source_text_len, 0, source_text_len, CRE2_UNANCHORED, pmatch, 3);
    memset(match_indices, 0xFF, sizeof(match_indices));
    set_matches = cre2_set_match(set, text_buf, (size_t)source_text_len, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));
    prog_size = cre2_program_size(re);

    // step 4: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rc_match + (long)set_compile_ret + (long)set_matches + (long)prog_size + (long)pmatch[0].length + (long)match_indices[0];
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}