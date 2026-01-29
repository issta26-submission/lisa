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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello) (world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hello world trailing";
    char workbuf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_string_t input;
    cre2_string_t rewrite;
    cre2_encoding_t enc;
    int rev = 0;
    int match_result = 0;
    int consume_result = 0;
    int computed_sum = 0;

    // step 2: Setup
    memset(workbuf, 0, sizeof(workbuf));
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    input.data = text;
    input.length = (int)(sizeof(text) - 1);
    rewrite.data = workbuf;
    rewrite.length = 0;

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    rev = cre2_version_interface_revision();

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_result = cre2_match(re, text, input.length, 0, input.length, CRE2_UNANCHORED, matches, 3);
    consume_result = cre2_find_and_consume_re(re, &input, &rewrite, 3);

    // step 5: Validate
    computed_sum = match_result + consume_result + rev + (int)enc + matches[0].length + matches[1].length + matches[2].length + input.length + rewrite.length;
    (void)computed_sum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}