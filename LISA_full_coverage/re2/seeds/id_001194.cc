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
//<ID> 1194
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
    char input_buf[] = "prefix foo123 suffix";
    const int input_len = (int)(sizeof(input_buf) - 1);
    char consume_buf[] = "foo456 remainder";
    const int consume_len = (int)(sizeof(consume_buf) - 1);
    char replace_buf[] = "foo123 foo45 foo789";
    const int replace_len = (int)(sizeof(replace_buf) - 1);
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input_str;
    cre2_string_t consume_str;
    cre2_string_t replace_text;
    cre2_string_t rewrite_str;
    cre2_string_t partial_matches[3];
    cre2_string_t consume_matches[3];
    int pm_ret = 0;
    int cons_ret = 0;
    int rep_ret = 0;
    int grep_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Compile and prepare buffers
    re = cre2_new(pattern, pattern_len, opt);
    memset(&input_str, 0, sizeof(input_str));
    memset(&consume_str, 0, sizeof(consume_str));
    memset(&replace_text, 0, sizeof(replace_text));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(consume_matches, 0, sizeof(consume_matches));
    input_str.data = input_buf;
    input_str.length = input_len;
    consume_str.data = consume_buf;
    consume_str.length = consume_len;
    replace_text.data = replace_buf;
    replace_text.length = replace_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 4: Operate
    pm_ret = cre2_partial_match(pattern, &input_str, partial_matches, 3);
    cons_ret = cre2_consume(pattern, &consume_str, consume_matches, 3);
    rep_ret = cre2_replace_re(re, &replace_text, &rewrite_str);
    grep_ret = cre2_global_replace_re(re, &replace_text, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)input_str.length + (long)consume_str.length
               + (long)partial_matches[0].length + (long)consume_matches[0].length
               + (long)pm_ret + (long)cons_ret + (long)rep_ret + (long)grep_ret;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}