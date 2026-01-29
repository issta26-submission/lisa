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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Prepare options and set encoding
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        // set encoding to UTF-8 for this options object
        cre2_opt_set_encoding(opt, CRE2_UTF8);
    }

    // Verify encoding (use cre2_opt_encoding)
    cre2_encoding_t enc = CRE2_UNKNOWN;
    if (opt) enc = cre2_opt_encoding(opt);
    (void)enc; // suppress unused variable warnings if any

    // Prepare a pattern with a named capturing group and a second alternative
    const char pattern[] = "(?P<name>foo)|(bar)";
    cre2_regexp_t *rex = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);

    // If regex compiled, enumerate named groups using the iterator APIs
    cre2_named_groups_iter_t *iter = nullptr;
    const char *last_name = nullptr;
    int last_index = -1;
    if (rex) {
        iter = cre2_named_groups_iter_new(rex);
        if (iter) {
            const char *ng_name = nullptr;
            int ng_index = 0;
            // Use cre2_named_groups_iter_next to walk named groups
            while (cre2_named_groups_iter_next(iter, &ng_name, &ng_index)) {
                // record last seen name/index (no I/O here)
                last_name = ng_name;
                last_index = ng_index;
            }
            // Delete the iterator using the required API
            cre2_named_groups_iter_delete(iter);
            iter = nullptr;
        }
    }

    // Demonstrate cre2_quote_meta: quote a string containing regex meta characters
    const char original_text[] = "a.b*c+?^$[z]";
    int orig_len = (int)(sizeof(original_text) - 1);
    // Allocate a buffer large enough to hold a quoted version (worst-case ~2x)
    int quoted_cap = orig_len * 2 + 10;
    char *quoted_buf = nullptr;
    if (quoted_cap > 0) quoted_buf = new char[quoted_cap];

    cre2_string_t original;
    original.data = original_text;
    original.length = orig_len;

    cre2_string_t quoted;
    quoted.data = quoted_buf ? quoted_buf : "";
    quoted.length = quoted_cap; // supply capacity in length field (implementation-specific)

    // Call cre2_quote_meta (result ignored except to exercise the API)
    if (quoted_buf) {
        (void)cre2_quote_meta(&quoted, &original);
    }

    // Prepare a text to perform a global replace on, and a rewrite string
    const char input_text[] = "foo and foo and bar";
    cre2_string_t text_and_target;
    text_and_target.data = input_text;
    text_and_target.length = (int)(sizeof(input_text) - 1);

    const char rewrite_text[] = "REPLACED";
    cre2_string_t rewrite;
    rewrite.data = rewrite_text;
    rewrite.length = (int)(sizeof(rewrite_text) - 1);

    // Use cre2_global_replace_re to perform replacement using the compiled regex
    if (rex) {
        (void)cre2_global_replace_re(rex, &text_and_target, &rewrite);
    }

    // Clean up resources
    if (rex) cre2_delete(rex);
    if (opt) cre2_opt_delete(opt);
    delete[] quoted_buf;

    // Return the required success code
    return 66;
}