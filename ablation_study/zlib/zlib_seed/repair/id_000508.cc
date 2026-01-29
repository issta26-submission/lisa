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
//<ID> 508
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input[128];
    memset(input, 'Z', sizeof(input));
    uInt in_len = (uInt)sizeof(input);
    uInt half_len = in_len / 2;
    uLong ad1 = adler32(1UL, input, half_len);
    uLong ad2 = adler32(1UL, input + half_len, in_len - half_len);
    uLong ad_combined = adler32_combine64(ad1, ad2, (off64_t)(in_len - half_len));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = input;
    strm.avail_in = in_len;
    Bytef outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Operate
    uLong crc_full = crc32(0UL, input, in_len);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzf = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzf, input, (unsigned int)in_len);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&strm);
    (void)ver;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)inf_init_ret;
    (void)crc_full;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}