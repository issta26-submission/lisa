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
//<ID> 843
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char target_buf[] = "xxabczz";
    const int target_len = (int)(sizeof(target_buf) - 1);
    char consume_buf[] = "abc";
    const int consume_len = (int)(sizeof(consume_buf) - 1);
    char rewrite_buf[] = "R";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);

    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;

    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t consume_text;
    cre2_string_t consume_out[3];

    int add_res = 0;
    int compile_res = 0;
    int gr_res = 0;
    int cons_res = 0;
    int ver_rev = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate - prepare set and regex, perform consume and global replace
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);

    re = cre2_new(pattern, pattern_len, opt);

    memset(consume_out, 0, sizeof(consume_out));
    memset(&consume_text, 0, sizeof(consume_text));
    consume_text.data = consume_buf;
    consume_text.length = consume_len;
    cons_res = cre2_consume(pattern, &consume_text, consume_out, 3);

    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = target_buf;
    text_and_target.length = target_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    gr_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate - combine results into a checksum
    ver_rev = cre2_version_interface_revision();
    computed_check = (long)gr_res + (long)cons_res + (long)ver_rev + (long)add_res + (long)compile_res + (long)text_and_target.length + (long)consume_out[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}