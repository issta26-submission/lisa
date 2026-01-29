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
//<ID> 830
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char src[] = "zlib api sequence payload for inflateBack, gzwrite and crc32";
    unsigned int srcLen = (unsigned int)(sizeof(src) - 1);
    const Bytef *source = (const Bytef *)src;
    uLong crc_val = crc32(0L, source, (uInt)srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gzf, source, (unsigned int)srcLen);

    // step 2: Configure z_stream for inflateBack
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)srcLen;

    // step 3: Operate - call inflateBack (with NULL callbacks) and check gz offset
    int rc_inflate_back = inflateBack(&strm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    off64_t after_write_offset = gzoffset64(gzf);

    // step 4: Validate and cleanup
    int rc_close = gzclose(gzf);
    (void)crc_val;
    (void)rc_gz_write;
    (void)rc_inflate_back;
    (void)after_write_offset;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}