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
//<ID> 817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char initial_pattern[] = "foo";
    const int initial_pattern_len = (int)(sizeof(initial_pattern) - 1);
    char text_buf[] = "foo bar foo";
    const int text_buf_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char * extracted_pattern = NULL;
    int replace_result = 0;
    int computed_check = 0;

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate
    re = cre2_new(initial_pattern, initial_pattern_len, opt);
    extracted_pattern = cre2_pattern(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_buf_len;
    rewrite.data = "baz";
    rewrite.length = 3;
    replace_result = cre2_replace(extracted_pattern, &text_and_target, &rewrite);

    // step 4: Validate
    computed_check = replace_result + text_and_target.length + rewrite.length + (int)extracted_pattern[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}