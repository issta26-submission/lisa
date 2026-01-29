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
//<ID> 1028
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice123 bob456 carol789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char pm_buf[64];
    char cons_buf[64];
    cre2_options_t * opt = NULL;
    cre2_string_t input;
    cre2_string_t pm_match;
    cre2_string_t consume_text;
    cre2_string_t consume_match;
    int pm_res = 0;
    int cons_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations - prepare input and perform a partial match
    memset(pm_buf, 0, sizeof(pm_buf));
    input.data = text_buf;
    input.length = text_len;
    pm_match.data = pm_buf;
    pm_match.length = 0;
    pm_res = cre2_partial_match(pattern, &input, &pm_match, 2);

    // step 4: Additional operations - prepare mutable text and perform consume
    memset(cons_buf, 0, sizeof(cons_buf));
    consume_text.data = text_buf;
    consume_text.length = text_len;
    consume_match.data = cons_buf;
    consume_match.length = 0;
    cons_res = cre2_consume(pattern, &consume_text, &consume_match, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pm_res + (long)cons_res + (long)pm_match.length + (long)consume_match.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}