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
//<ID> 446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int deflate_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    unsigned char window[32768];
    int inflate_back_init_ret = inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef input[] = "Example payload for zlib deflate + crc + gzwrite sequence.";
    uInt input_len = (uInt)(sizeof(input) - 1);
    uLong bound = deflateBound(&defstrm, (uLong)input_len);
    Bytef *outbuf = (Bytef *)malloc((size_t)bound);
    defstrm.next_in = input;
    defstrm.avail_in = input_len;
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Operate
    int deflate_ret = deflate(&defstrm, 4); // Z_FINISH typically 4
    uLong compressed_len = defstrm.total_out;
    uLong crc_orig = crc32(0UL, input, input_len);
    uLong crc_combined = crc32(crc_orig, outbuf, (uInt)compressed_len);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, outbuf, (unsigned int)compressed_len);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_back_end_ret = inflateBackEnd(&backstrm);
    free(outbuf);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_back_init_ret;
    (void)bound;
    (void)deflate_ret;
    (void)compressed_len;
    (void)crc_orig;
    (void)crc_combined;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_back_end_ret;
    // API sequence test completed successfully
    return 66;
}