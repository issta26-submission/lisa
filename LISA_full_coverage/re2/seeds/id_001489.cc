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
//<ID> 1489
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abxyz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text;
    cre2_string_t matches[3];
    int consume_ret = 0;
    int opt_posix = 0;
    const char * re_pat = NULL;
    int errcode = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    opt_posix = cre2_opt_posix_syntax(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    text.data = text_buf;
    text.length = text_len;
    consume_ret = cre2_consume(pattern, &text, matches, 3);
    re_pat = re ? cre2_pattern(re) : NULL;
    errcode = re ? cre2_error_code(re) : 0;

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)consume_ret + (long)opt_posix
               + (long)(matches[0].length) + (long)(matches[1].length) + (long)(re != NULL)
               + (long)errcode + (long)(re_pat ? (int)strlen(re_pat) : 0);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}