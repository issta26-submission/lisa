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
//<ID> 1193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123 middle foo45 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char consume_buf[] = "foo999rest";
    const int consume_len = (int)(sizeof(consume_buf) - 1);
    char replace_text_buf[128];
    memset(replace_text_buf, 0, sizeof(replace_text_buf));
    char global_replace_text_buf[128];
    memset(global_replace_text_buf, 0, sizeof(global_replace_text_buf));
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;

    cre2_string_t text_str;
    cre2_string_t consume_str;
    cre2_string_t replace_text;
    cre2_string_t global_replace_text;
    cre2_string_t rewrite_str;
    cre2_string_t matches[4];
    cre2_string_t consume_matches[3];

    int pm_ret = 0;
    int cons_ret = 0;
    int repl_ret = 0;
    int grepl_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // step 3: Compile and prepare buffers
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    memset(consume_matches, 0, sizeof(consume_matches));
    text_str.data = text_buf;
    text_str.length = text_len;
    consume_str.data = consume_buf;
    consume_str.length = consume_len;
    replace_text.data = replace_text_buf;
    replace_text.length = 0;
    global_replace_text.data = global_replace_text_buf;
    global_replace_text.length = 0;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 4: Operate (partial match, consume, single replace, global replace)
    pm_ret = cre2_partial_match(pattern, &text_str, matches, (int)(sizeof(matches) / sizeof(matches[0])));
    cons_ret = cre2_consume(pattern, &consume_str, consume_matches, (int)(sizeof(consume_matches) / sizeof(consume_matches[0])));
    repl_ret = cre2_replace_re(re, &replace_text, &rewrite_str);
    grepl_ret = cre2_global_replace_re(re, &global_replace_text, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)pm_ret + (long)cons_ret + (long)repl_ret + (long)grepl_ret
                     + (long)matches[0].length + (long)consume_matches[0].length + (long)replace_text.length + (long)global_replace_text.length + (long)rewrite_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}