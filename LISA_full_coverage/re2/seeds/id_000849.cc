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
//<ID> 849
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t consume_in;
    cre2_string_t consume_out[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int match_arr[4];
    size_t set_matches = 0;
    int add_res = 0;
    int compile_res = 0;
    int consume_res = 0;
    int replace_res = 0;
    int rev = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate on set and do a match
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    memset(match_arr, 0, sizeof(match_arr));
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, (size_t)(sizeof(match_arr) / sizeof(match_arr[0])));

    // step 4: Create regexp, consume and do a global replace using the regexp
    re = cre2_new(pattern, pattern_len, opt);
    memset(&consume_in, 0, sizeof(consume_in));
    memset(consume_out, 0, sizeof(consume_out));
    consume_in.data = text_buf;
    consume_in.length = text_len;
    consume_res = cre2_consume(pattern, &consume_in, consume_out, 3);
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / compute a lightweight checksum using version info and results
    rev = cre2_version_interface_revision();
    computed_check = (long)rev + (long)add_res + (long)compile_res + (long)set_matches + (long)consume_res + (long)replace_res + (long)match_arr[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}