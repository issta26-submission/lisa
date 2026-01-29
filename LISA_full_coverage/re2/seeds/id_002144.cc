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
//<ID> 2144
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
    char source_buf[] = "abc-123 xyz-456";
    const int source_len = (int)(sizeof(source_buf) - 1);
    cre2_string_t text = { source_buf, source_len };
    char match0_buf[64];
    char match1_buf[64];
    memset(match0_buf, 0, sizeof(match0_buf));
    memset(match1_buf, 0, sizeof(match1_buf));
    cre2_string_t matches[4];
    matches[0].data = match0_buf;
    matches[0].length = 0;
    matches[1].data = match1_buf;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;
    matches[3].data = NULL;
    matches[3].length = 0;
    cre2_options_t * opt = NULL;
    int easy_ret = 0;
    int partial_ret = 0;
    int literal_flag = 0;
    int cs_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Configure (observe option values)
    literal_flag = cre2_opt_literal(opt);
    cs_flag = cre2_opt_case_sensitive(opt);

    // step 4: Operate (perform easy match and partial match)
    easy_ret = cre2_easy_match(pattern, pattern_len, text.data, text.length, matches, 3);
    partial_ret = cre2_partial_match(pattern, &text, matches, 3);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)literal_flag + (long)cs_flag + (long)matches[0].length + (long)matches[1].length + (long)text.length + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}