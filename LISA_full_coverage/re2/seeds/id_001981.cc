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
//<ID> 1981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "^hello";
    const char pattern2[] = "world$";
    const size_t pattern1_len = (size_t)(sizeof(pattern1) - 1);
    const size_t pattern2_len = (size_t)(sizeof(pattern2) - 1);
    const char text[] = "hello brave new world";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    char errorbuf[128];
    int add_ret1 = 0;
    int add_ret2 = 0;
    int match_indices[8];
    size_t nmatched = 0;
    long checksum = 0;

    // step 2: Setup
    memset(errorbuf, 0, sizeof(errorbuf));
    memset(match_indices, 0xFF, sizeof(match_indices)); // initialize to -1 pattern for clarity
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Create set and add patterns
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret1 = cre2_set_add(set, pattern1, pattern1_len, errorbuf, sizeof(errorbuf));
    add_ret2 = cre2_set_add(set, pattern2, pattern2_len, errorbuf, sizeof(errorbuf));

    // step 4: Match and validate
    nmatched = cre2_set_match(set, text, text_len, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));
    checksum = (long)pattern1_len + (long)pattern2_len + (long)text_len + (long)add_ret1 + (long)add_ret2 + (long)nmatched + (long)match_indices[0] + (long)match_indices[1] + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}