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
//<ID> 2140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(\\w+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abc-123 xyz-456";
    int text_len = (int)(sizeof(text_buf) - 1);
    char pmatch_buf[64];
    cre2_options_t * opt = NULL;
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t ematch[4];
    cre2_string_t pmatch = { pmatch_buf, 0 };
    int easy_ret = 0;
    int partial_ret = 0;
    int literal_flag = 0;
    int case_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    literal_flag = cre2_opt_literal(opt);
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);
    case_flag = cre2_opt_case_sensitive(opt);

    // step 3: Prepare match storage
    memset(ematch, 0, sizeof(ematch));
    memset(pmatch_buf, 0, sizeof(pmatch_buf));
    pmatch.data = pmatch_buf;
    pmatch.length = 0;

    // step 4: Operate (perform easy_match and partial_match)
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, ematch, 3);
    partial_ret = cre2_partial_match(pattern, &text, &pmatch, 3);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)literal_flag + (long)case_flag + (long)pmatch.length + (long)text.length + (long)(opt != NULL) + (long)ematch[0].length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}