#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    gzFile gz;
    const char msg1[] = "First write via gzwrite: Hello zlib!";
    const char msg2[] = "Second write after rewind: Goodbye zlib!";
    unsigned int len1 = (unsigned int)(sizeof(msg1) - 1);
    unsigned int len2 = (unsigned int)(sizeof(msg2) - 1);
    int ret_gzwrite1;
    int ret_gzrewind;
    int ret_gzwrite2;
    int ret_gzclose;
    off_t offset_after_first;
    off_t offset_after_second;
    const char *ver;
    char scratch[128];

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));

    // step 3: Core operations
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzwrite1 = gzwrite(gz, msg1, len1);
    offset_after_first = gzoffset(gz);
    ret_gzrewind = gzrewind(gz);
    ret_gzwrite2 = gzwrite(gz, msg2, len2);
    offset_after_second = gzoffset(gz);

    // step 4: Validate and cleanup
    ver = zlibVersion();
    ret_gzclose = gzclose(gz);

    (void)ret_gzwrite1;
    (void)ret_gzrewind;
    (void)ret_gzwrite2;
    (void)ret_gzclose;
    (void)offset_after_first;
    (void)offset_after_second;
    (void)ver;
    (void)scratch;
    // API sequence test completed successfully
    return 66;
}