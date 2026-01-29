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
//<ID> 2092
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(Hello) (World)([0-9]{1,3})$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Hello World42";
    int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t input = { text_buf, text_len };
    cre2_string_t matches[4];
    int partial_result = 0;
    int ver_current = 0;
    int64_t max_mem = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Configure (inspect options and compile regexp)
    max_mem = cre2_opt_max_mem(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (clear match storage and perform a partial match)
    memset(matches, 0, sizeof(matches));
    partial_result = cre2_partial_match_re(re, &input, matches, 4);

    // step 5: Validate (fetch version info and combine results)
    ver_current = cre2_version_interface_current();
    checksum = (long)pattern_len + (long)partial_result + (long)ver_current + (long)max_mem + (long)matches[0].length + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}