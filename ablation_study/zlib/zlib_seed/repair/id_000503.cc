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
//<ID> 503
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef inbuf[256];
    memset(inbuf, 'A', sizeof(inbuf));
    uLong crc_orig = crc32(0UL, inbuf, (uInt)sizeof(inbuf));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure (write compressed .gz using gzopen/gzwrite)
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, inbuf, (unsigned int)sizeof(inbuf));
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");

    // step 3: Operate (read decompressed data back and compute checksums)
    Bytef outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    int read_len = gzread(gzr, outbuf, (unsigned int)sizeof(outbuf));
    uLong crc_read = crc32(0UL, outbuf, (uInt)read_len);
    uLong ad1 = adler32(1UL, outbuf, (uInt)read_len);
    uLong ad2 = adler32(0UL, outbuf, (uInt)read_len);
    uLong combined_adler = adler32_combine64(ad1, ad2, (off64_t)read_len);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzclose_r_ret = gzclose(gzr);

    (void)ver;
    (void)crc_orig;
    (void)inf_init_ret;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)read_len;
    (void)crc_read;
    (void)ad1;
    (void)ad2;
    (void)combined_adler;
    (void)inf_end_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}