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
//<ID> 1716
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
    const char source_text[] = "this is testing text";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set_ptr = NULL;
    cre2_string_t pmatch[4];
    int rc_match = 0;
    int program_sz = 0;
    int set_compile_rc = 0;
    int match_ints[8];
    size_t set_matches = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    set_ptr = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    memset(pmatch, 0, sizeof(pmatch));
    memset(match_ints, 0, sizeof(match_ints));
    cre2_set_add_simple(set_ptr, pattern);

    // step 4: Operate
    program_sz = cre2_program_size(re);
    set_compile_rc = cre2_set_compile(set_ptr);
    rc_match = cre2_match(re, source_text, source_text_len, 0, source_text_len, CRE2_UNANCHORED, pmatch, 4);
    set_matches = cre2_set_match(set_ptr, source_text, (size_t)source_text_len, match_ints, (size_t)8);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)program_sz + (long)rc_match + (long)set_compile_rc + (long)set_matches + (long)pmatch[0].length + (long)match_ints[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set_ptr);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}