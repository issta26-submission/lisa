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
//<ID> 1647
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)|(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char input_text[] = "head foo middle bar foo tail";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    char textmod[] = "head foo middle bar foo tail";
    const int textmod_initial_len = (int)(sizeof(textmod) - 1);
    char rewrite_buf[] = "REPL";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char extracted_buf[64] = { 0 };
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    const char * compiled_pat = NULL;
    cre2_string_t text_str;
    cre2_string_t textmod_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    int match[8];
    size_t nmatched = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    text_str.data = input_text;
    text_str.length = input_text_len;
    textmod_str.data = textmod;
    textmod_str.length = textmod_initial_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    target_str.data = extracted_buf;
    target_str.length = 0;

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    (void)compiled_pat;
    cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    cre2_global_replace_re(re, &textmod_str, &rewrite_str);
    memset(match, -1, sizeof(match));
    nmatched = cre2_set_match(set, textmod_str.data, (size_t)textmod_str.length, match, (size_t)(sizeof(match) / sizeof(match[0])));

    // step 5: Validate
    checksum = (long)pattern_len + (long)(compiled_pat != NULL) + (long)nmatched + (long)match[0] + (long)match[1]
               + (long)text_str.length + (long)textmod_str.length + (long)rewrite_str.length + (long)target_str.length
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}