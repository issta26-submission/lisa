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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "Example data for zlib compression -> write -> inflate sequence.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[512];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[512];
    gzFile gzf;
    const char * ver;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate;
    int bytes_written;
    int ret_gzclose;
    int ret_inflate_end;

    // step 2: Setup (initialize stream and create compressed data, open gz file)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    bytes_written = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Core operations (configure stream with compressed data and inflate)
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&strm, 0);

    // step 4: Cleanup (end inflate and close gz file)
    ret_inflate_end = inflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)bytes_written;
    (void)ret_inflate_end;
    (void)ret_gzclose;
    (void)ver;
    (void)input_len;
    (void)comp_len;
    (void)strm;

    // API sequence test completed successfully
    return 66;
}