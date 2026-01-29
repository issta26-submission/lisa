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
//<ID> 2183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>\\w+)-(?P<num>\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-100 Bob-200";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text_and_target;
    cre2_string_t match;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pattern = NULL;
    int perl_flag = 0;
    int consume_ret = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 3: Configure (compile regexp and prepare strings)
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;

    // step 4: Operate (attempt to consume from the text using the compiled pattern)
    consume_ret = cre2_consume(compiled_pattern, &text_and_target, &match, 0);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)consume_ret + (long)perl_flag + (long)text_and_target.length + (long)match.length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}