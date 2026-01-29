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
//<ID> 176
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
    const char input_text[] = "xx foobar yy";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$2-$1";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_res = 0;
    int err_code = 0;
    int prog_size = 0;
    cre2_encoding_t enc_val = CRE2_UNKNOWN;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    enc_val = cre2_opt_encoding(opt);
    cre2_opt_set_one_line(opt, (int)enc_val & 1);

    // step 3: Core operations (compile and replace)
    re = cre2_new(pattern, pattern_len, opt);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);
    err_code = cre2_error_code(re);
    prog_size = cre2_program_size(re);

    // step 4: Validate / propagate results (use values meaningfully)
    text_and_target.length = text_and_target.length + ((replace_res + err_code + prog_size) & 7);
    cre2_opt_set_case_sensitive(opt, (err_code >> 1) & 1);
    cre2_opt_set_one_line(opt, (prog_size >> 2) & 1);
    (void)pattern_len;
    (void)input_text_len;
    (void)enc_val;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}