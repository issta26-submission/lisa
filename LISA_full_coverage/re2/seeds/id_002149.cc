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
//<ID> 2149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Bob42 and Alice123";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text = { text_buf, text_len };
    cre2_options_t * opt = NULL;
    cre2_string_t matches[3];
    cre2_string_t pmatch[3];
    int easy_ret = 0;
    int partial_ret = 0;
    int literal_flag = 0;
    int cs_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    cs_flag = cre2_opt_case_sensitive(opt);

    // step 3: Initialize match containers
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    pmatch[0].data = NULL; pmatch[0].length = 0;
    pmatch[1].data = NULL; pmatch[1].length = 0;
    pmatch[2].data = NULL; pmatch[2].length = 0;

    // step 4: Operate (perform easy match and partial match)
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    partial_ret = cre2_partial_match(pattern, &text, pmatch, 3);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)matches[0].length + (long)matches[1].length + (long)pmatch[0].length + (long)literal_flag + (long)cs_flag + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}