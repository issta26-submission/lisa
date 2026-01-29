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
//<ID> 441
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
    memset(window, 0, sizeof(window));
    int inflate_back_init_ret = inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef input[] = "Example data to be compressed by deflate and then written via gzwrite.";
    uInt input_len = (uInt)(sizeof(input) - 1);
    defstrm.next_in = input;
    defstrm.avail_in = input_len;
    uLong bound_est = deflateBound(&defstrm, (uLong)input_len);
    Bytef *outbuf = new Bytef[(size_t)bound_est + 1];
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)((bound_est + 1) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(bound_est + 1));

    // step 3: Operate
    int deflate_ret = deflate(&defstrm, 4); // Z_FINISH == 4
    unsigned int comp_len = (unsigned int)defstrm.total_out;
    uLong crc = crc32(0UL, outbuf, (uInt)comp_len);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, outbuf, comp_len);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_back_end_ret = inflateBackEnd(&backstrm);
    delete [] outbuf;
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_back_init_ret;
    (void)bound_est;
    (void)deflate_ret;
    (void)crc;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_back_end_ret;
    // API sequence test completed successfully
    return 66;
}