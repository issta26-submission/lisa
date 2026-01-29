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
//<ID> 2189
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
    cre2_string_t subject;
    cre2_string_t match;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * retrieved_pattern = NULL;
    int perl_flag = 0;
    int consume_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&subject, 0, sizeof(subject));
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure
    perl_flag = cre2_opt_perl_classes(opt);
    re = cre2_new(pattern, pattern_len, opt);
    retrieved_pattern = cre2_pattern(re);
    subject.data = text_buf;
    subject.length = text_len;

    // step 4: Operate
    consume_ret = cre2_consume(pattern, &subject, &match, 3);

    // step 5: Validate
    checksum = (long)pattern_len + (long)consume_ret + (long)perl_flag + (long)(opt != NULL) + (long)(re != NULL) + (long)subject.length + (long)match.length + (long)(retrieved_pattern != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}