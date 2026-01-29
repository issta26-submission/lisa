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
//<ID> 1275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<id>foo)(bar[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foobar42 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    int easy_ret = 0;
    int named_idx = 0;
    int prog_size = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(matches, 0, sizeof(matches));

    // step 3: Configure
    int one_line_flag = cre2_opt_one_line(opt);
    (void)one_line_flag;

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);
    named_idx = cre2_find_named_capturing_groups(re, "id");
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)prog_size + (long)named_idx
                     + (long)easy_ret + (long)(opt != NULL) + (long)(re != NULL)
                     + (long)(matches[0].length) + (long)(matches[1].length) + (long)(matches[2].length);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}