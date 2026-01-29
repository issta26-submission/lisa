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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    unsigned char dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    unsigned char inbuf[] = "Hello, zlib with dictionary!";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    unsigned char outbuf[512];
    uInt out_buf_size = (uInt)sizeof(outbuf);
    int ret_init;
    int ret_setdict;
    int ret_deflate;
    int ret_getdict;
    int ret_reset;
    int ret_end;
    int bytes_written;
    uInt got_dict_len;
    gzFile gzf;
    off64_t seek_res;

    // step 2: Setup (initialize stream and open gzip file)
    memset(&strm, 0, sizeof(strm));
    ret_init = deflateInit_(&strm, 6, zlibVersion(), sizeof(z_stream));
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "w+b");

    // step 3: Operate & Validate (set dictionary, compress, retrieve dictionary, reset, seek)
    ret_setdict = deflateSetDictionary(&strm, (const Bytef *)dict, dict_len);
    strm.next_in = (Bytef *)inbuf;
    strm.avail_in = in_len;
    strm.next_out = (Bytef *)outbuf;
    strm.avail_out = out_buf_size;
    ret_deflate = deflate(&strm, 0);
    got_dict_len = out_buf_size;
    ret_getdict = deflateGetDictionary(&strm, outbuf, &got_dict_len);
    ret_reset = deflateReset(&strm);
    bytes_written = gzwrite(gzf, outbuf, (unsigned int)strm.total_out);
    seek_res = gzseek64(gzf, (off64_t)strm.total_out, 0);

    // step 4: Cleanup
    ret_end = deflateEnd(&strm);
    gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_init; (void)ret_setdict; (void)ret_deflate; (void)ret_getdict; (void)ret_reset; (void)ret_end;
    (void)bytes_written; (void)got_dict_len; (void)seek_res;
    return 66;
}