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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "start helloworld end";
    const char rewrite_template[] = "$2,$1";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int program_size_val = 0;
    int err_code = 0;
    int replace_res = 0;
    cre2_encoding_t enc_val = CRE2_UNKNOWN;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = input_text;
    text_and_target.length = (int)(sizeof(input_text) - 1);
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Core operations (compile, query, replace)
    enc_val = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // Guard against NULL regexp to avoid dereferencing null and to ensure encoding selection stays valid
    if (re != NULL) {
        program_size_val = cre2_program_size(re);
        err_code = cre2_error_code(re);
        replace_res = cre2_replace_re(re, &text_and_target, &rewrite);
    } else {
        program_size_val = 0;
        err_code = 0;
        replace_res = 0;
    }

    // step 4: Validate / propagate results (use API outputs meaningfully)
    text_and_target.length = text_and_target.length + ((program_size_val + err_code + replace_res + (int)enc_val) & 7);
    cre2_opt_set_one_line(opt, (err_code + replace_res) & 1);
    cre2_opt_set_case_sensitive(opt, (program_size_val >> 1) & 1);

    // Ensure the computed encoding value is within the valid enum range [0,2]
    {
        int enc_choice = (program_size_val + err_code + replace_res) % 3;
        if (enc_choice < 0) enc_choice += 3;
        cre2_opt_set_encoding(opt, (cre2_encoding_t)enc_choice);
    }
    (void)pattern_len;

    // step 5: Cleanup
    if (re != NULL) cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}