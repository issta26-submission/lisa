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
//<ID> 874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * roundtrip_pattern = NULL;
    cre2_string_t match_out[3];
    int easy_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);

    // step 3: Operate (compile and round-trip pattern, then easy match)
    re = cre2_new(pattern, pattern_len, opt);
    roundtrip_pattern = cre2_pattern(re);
    memset(match_out, 0, sizeof(match_out));
    easy_res = cre2_easy_match(roundtrip_pattern, pattern_len, text_buf, text_len, match_out, 3);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)match_out[0].length + (long)match_out[1].length + (long)match_out[2].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}