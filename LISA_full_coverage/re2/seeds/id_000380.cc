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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(bc)?d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxabcdyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_sp;
    cre2_string_t matches[4];
    char errbuf[128];
    int consume_res = 0;
    int set_add_res = 0;
    int errcode = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(errbuf, 0, sizeof(errbuf));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    errcode = cre2_error_code(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_res = cre2_set_add(set, pattern, (size_t)pattern_len, errbuf, sizeof(errbuf));
    consume_res = cre2_consume(pattern, &text_sp, matches, 4);

    // step 5: Validate
    computed_check = consume_res + set_add_res + errcode + matches[0].length + text_sp.length + pattern_len + (int)errbuf[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}