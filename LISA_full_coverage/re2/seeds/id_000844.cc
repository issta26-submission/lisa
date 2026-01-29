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
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1-$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t consume_text;
    cre2_string_t consume_out;
    int case_flag = 1;
    int add_res = 0;
    int compile_res = 0;
    int consume_res = 0;
    int replace_res = 0;
    int ver_rev = 0;
    long computed_check = 0;

    // Step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&consume_text, 0, sizeof(consume_text));
    memset(&consume_out, 0, sizeof(consume_out));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    consume_text.data = text_buf;
    consume_text.length = text_len;
    consume_res = cre2_consume(pattern, &consume_text, &consume_out, 3);
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);
    ver_rev = cre2_version_interface_revision();

    // Step 4: Validate
    computed_check = (long)case_flag + (long)add_res + (long)compile_res + (long)consume_res + (long)replace_res + (long)ver_rev + (long)text_and_target.length + (long)consume_out.length;
    (void)computed_check;

    // Step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}