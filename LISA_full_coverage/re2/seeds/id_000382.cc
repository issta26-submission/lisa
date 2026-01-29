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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobarbazyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_sp;
    cre2_string_t consumed;
    char set_errbuf[64];
    int set_add_res = 0;
    int consume_res = 0;
    int err_code = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&consumed, 0, sizeof(consumed));
    memset(set_errbuf, 0, sizeof(set_errbuf));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    /* cre2_set_new expects a cre2_anchor_t for the second argument.
       Cast 0 to cre2_anchor_t to avoid a type-mismatch in C++ builds. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    set_add_res = cre2_set_add(set, pattern, (size_t)pattern_len, set_errbuf, sizeof(set_errbuf));
    consume_res = cre2_consume(pattern, &text_sp, &consumed, 1);
    if (re) err_code = cre2_error_code(re);

    // step 5: Validate
    computed_check = set_add_res + consume_res + err_code + consumed.length + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}