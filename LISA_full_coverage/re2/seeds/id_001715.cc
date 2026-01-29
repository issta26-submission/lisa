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
//<ID> 1715
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
    const char source_text[] = "foobarbaz";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t pmatch[3];
    int match_indices[16];
    int rc_match = 0;
    int prog_size = 0;
    size_t set_found = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    memset(pmatch, 0, sizeof(pmatch));
    rc_match = cre2_match(re, source_text, source_text_len, 0, source_text_len, CRE2_UNANCHORED, pmatch, 3);
    prog_size = cre2_program_size(re);
    memset(match_indices, 0, sizeof(match_indices));
    set_found = cre2_set_match(set, source_text, (size_t)source_text_len, match_indices, (size_t)(sizeof(match_indices) / sizeof(match_indices[0])));

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rc_match + (long)prog_size + (long)set_found + (long)pmatch[0].length + (long)match_indices[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}