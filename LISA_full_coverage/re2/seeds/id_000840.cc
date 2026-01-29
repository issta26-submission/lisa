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
//<ID> 840
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxhello123yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char replacement_buf[] = "X";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t consume_out[4];
    cre2_string_t consume_text;
    int add_res = 0;
    int compile_res = 0;
    int gr_res = 0;
    int cons_res = 0;
    int rev = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    memset(&text_and_target, 0, sizeof(text_and_target));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    memset(&rewrite, 0, sizeof(rewrite));
    rewrite.data = replacement_buf;
    rewrite.length = (int)(sizeof(replacement_buf) - 1);
    gr_res = cre2_global_replace_re(re, &text_and_target, &rewrite);
    memset(consume_out, 0, sizeof(consume_out));
    memset(&consume_text, 0, sizeof(consume_text));
    consume_text.data = text_buf;
    consume_text.length = text_len;
    cons_res = cre2_consume(pattern, &consume_text, consume_out, 4);

    // step 4: Additional checks
    rev = cre2_version_interface_revision();
    computed_check = (long)gr_res + (long)cons_res + (long)rev + (long)text_and_target.length + (long)rewrite.length + (long)consume_out[0].length + (long)add_res + (long)compile_res;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}