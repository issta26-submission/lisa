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
//<ID> 1848
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "xx foobar yy";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[128];
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[4];
    memset(matches, 0, sizeof(matches));
    matches[0].data = text_buf;
    matches[0].length = 0;
    matches[1].data = text_buf;
    matches[1].length = 0;
    matches[2].data = text_buf;
    matches[2].length = 0;
    matches[3].data = text_buf;
    matches[3].length = 0;
    const char * err_str = NULL;
    int easy_res = 0;
    int ver_iface = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    err_str = cre2_error_string(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Core operations
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, matches, 3);
    ver_iface = cre2_version_interface_current();

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)easy_res + (long)ver_iface + (long)(re != NULL) + (long)(opt != NULL) + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)(err_str != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}