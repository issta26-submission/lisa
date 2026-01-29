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
//<ID> 445
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
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int inf_back_init_ret = inflateBackInit_(&infstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef source[] = "Example data to compress via deflate and compute crc32.";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    uLong crc = crc32(0UL, source, (uInt)sourceLen);
    uLong bound = deflateBound(&defstrm, sourceLen);
    uInt out_buf_size = (uInt)bound;
    if (out_buf_size == 0) out_buf_size = 1; // ensure at least one byte for allocation without loops/branches elsewhere
    Bytef *dest = new Bytef[out_buf_size];
    memset(dest, 0, out_buf_size);
    defstrm.next_in = source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = dest;
    defstrm.avail_out = out_buf_size;

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong compressed_len = defstrm.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, dest, (unsigned int)compressed_len);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int def_end_ret = deflateEnd(&defstrm);
    int inf_back_end_ret = inflateBackEnd(&infstrm);
    delete [] dest;
    (void)ver;
    (void)def_init_ret;
    (void)inf_back_init_ret;
    (void)crc;
    (void)bound;
    (void)out_buf_size;
    (void)def_ret;
    (void)compressed_len;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)inf_back_end_ret;
    // API sequence test completed successfully
    return 66;
}