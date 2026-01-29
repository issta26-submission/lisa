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
//<ID> 2201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123bar baz foo456bar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int prog_size = 0;
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);

    // step 4: Operate
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)prog_size + (long)match_ret + (long)(enc) + (long)(matches[0].length) + (long)(matches[1].length) + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}