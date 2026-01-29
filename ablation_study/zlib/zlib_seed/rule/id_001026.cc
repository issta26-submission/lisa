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
//<ID> 1026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'G', (size_t)ORIG_LEN);
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 2: Create gzip file via file descriptor and gzdopen, write data, finalize writer
    FILE *wf = fopen("test_zlib_api_sequence.gz", "wb");
    int wfd = fileno(wf);
    gzFile gw = gzdopen(wfd, "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 0);
    gzclose_w(gw);
    fclose(wf);

    // step 3: Open gzip for reading, push back a byte with gzungetc, read data and compute CRCs
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzungetc((int)orig[0], gr);
    Bytef *readbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(readbuf, 0, (size_t)ORIG_LEN);
    int bytes_read = gzread(gr, readbuf, (unsigned int)ORIG_LEN);
    uLong crc_orig = crc32_z(0L, orig, (z_size_t)ORIG_LEN);
    uLong crc_read = crc32_z(0L, readbuf, (z_size_t)(bytes_read > 0 ? (uLong)bytes_read : 0));
    (void)crc_orig;
    (void)crc_read;

    // step 4: Cleanup resources
    gzclose(gr);
    free(orig);
    free(readbuf);
    return 66;
    // API sequence test completed successfully
}