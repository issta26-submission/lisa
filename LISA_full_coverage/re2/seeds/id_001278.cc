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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<myname>foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char name[] = "myname";
    char text_buf[] = "prefix foo123 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    int easy_ret = 0;
    int named_idx = 0;
    int prog_size = 0;
    int one_line_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Core operations - compile and inspect program
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);

    // step 4: Core operations - perform an easy match using the pattern string API
    memset(matches, 0, sizeof(matches));
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 4);

    // step 5: Core operations - find named capturing group index from compiled regexp
    named_idx = cre2_find_named_capturing_groups(re, name);

    // step 6: Validate / Cleanup
    computed_check = (long)pattern_len + (long)text_len + (long)prog_size + (long)easy_ret
                     + (long)named_idx + (long)one_line_flag + (long)(re != NULL)
                     + (long)matches[0].length + (long)matches[1].length;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}