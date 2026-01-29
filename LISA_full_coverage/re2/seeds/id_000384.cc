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
//<ID> 384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab|cd)ef";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "zzabefyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_sp;
    cre2_string_t matches[3];
    int consume_res = 0;
    int add_res = 0;
    int errcode = 0;
    int computed_check = 0;
    char set_errbuf[128];

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(set_errbuf, 0, sizeof(set_errbuf));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add(set, pattern, (size_t)pattern_len, set_errbuf, sizeof(set_errbuf));

    // step 4: Operate
    consume_res = cre2_consume(pattern, &text_sp, matches, 3);
    errcode = cre2_error_code(re);

    // step 5: Validate
    computed_check = add_res + consume_res + errcode + matches[0].length + pattern_len + text_len;
    (void)computed_check;
    (void)set_errbuf;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}