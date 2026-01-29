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
//<ID> 2184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456 Carol-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_string;
    cre2_string_t matches[4];
    int perl_flag = 0;
    int consume_ret = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 3: Configure (compile regexp and prepare strings)
    re = cre2_new(pattern, pattern_len, opt);
    memset(&text_string, 0, sizeof(text_string));
    memset(matches, 0, sizeof(matches));
    text_string.data = text_buf;
    text_string.length = text_len;

    // step 4: Operate (use compiled pattern string and perform consume)
    consume_ret = cre2_consume(cre2_pattern(re), &text_string, matches, 2);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)consume_ret + (long)perl_flag + (long)text_string.length + (long)matches[0].length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}