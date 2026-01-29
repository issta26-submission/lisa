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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Prepare a pattern with a few capturing groups.
    const char pattern[] = "(a)(b(c))";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile the regex. Pass NULL for options (use defaults).
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, NULL);
    if (!re) {
        // Failed to create regexp; still treat as completed for this test.
        return 66;
    }

    // Query number of capturing groups (does not include group 0).
    int num_groups = cre2_num_capturing_groups(re);
    // Allocate match slots including group 0.
    int nmatch = num_groups + 1;
    if (nmatch < 1) nmatch = 1;

    // Allocate arrays for matches and ranges.
    cre2_string_t *matches = new cre2_string_t[nmatch];
    cre2_range_t *ranges = new cre2_range_t[nmatch];
    // Initialize matches and ranges defensively.
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
        ranges[i].start = -1;
        ranges[i].past = -1;
    }

    // Prepare input text that begins with something matching the pattern.
    const char text_data[] = "abcc_extra";
    cre2_string_t text;
    text.data = text_data;
    text.length = (int)(sizeof(text_data) - 1);

    // Try to consume a match from the start of text.
    // On success, matches[] will be populated with the matched substrings.
    int consumed = cre2_consume_re(re, &text, matches, nmatch);

    if (consumed) {
        // Convert the matches (substrings) into ranges (start, past) relative to text_data.
        cre2_strings_to_ranges(text.data, ranges, matches, nmatch);

        // Basic validation of ranges (optional): ensure start/past are sane.
        for (int i = 0; i < nmatch; ++i) {
            long s = ranges[i].start;
            long p = ranges[i].past;
            if (s >= 0 && p >= s) {
                // valid range within the text; no-op for this test
                (void)s; (void)p;
            }
        }
    }

    // Cleanup
    delete [] matches;
    delete [] ranges;
    cre2_delete(re);

    return 66;
}