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
//<ID> 1861
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
    char rewrite_buf[32];
    char target_buf[128];
    memset(text_buf, 0, sizeof(text_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, "$1", 3);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;

    cre2_string_t matches[3];
    memset(matches, 0, sizeof(matches));
    matches[0].data = text_buf;
    matches[0].length = 0;
    matches[1].data = text_buf;
    matches[1].length = 0;
    matches[2].data = text_buf;
    matches[2].length = 0;

    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    text_str.data = text_buf;
    text_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 2;
    target_str.data = target_buf;
    target_str.length = 0;

    int easy_res = 0;
    int add1_res = 0;
    int add2_res = 0;
    int compile_res = 0;
    int extract_res = 0;
    const char * ver = NULL;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add1_res = cre2_set_add_simple(set, "foo");
    add2_res = cre2_set_add_simple(set, "bar");
    compile_res = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    ver = cre2_version_string();
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, source_text_len, matches, 3);
    extract_res = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    // step 4: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)(ver != NULL) + (long)easy_res + (long)extract_res + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)matches[0].length + (long)target_str.length + (long)add1_res + (long)add2_res + (long)compile_res;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}