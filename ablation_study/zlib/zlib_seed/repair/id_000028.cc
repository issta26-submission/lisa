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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    unsigned char inbuf[] = "Hello zlib dictionary test";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    unsigned char outbuf[512];
    Bytef dictionary[] = "preset_dictionary";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    Bytef fetched_dict[64];
    uInt fetched_len = (uInt)sizeof(fetched_dict);
    gzFile gzf;
    off64_t newpos;
    int ret_deflate_init;
    int ret_setdict;
    int ret_deflate;
    int ret_getdict;
    int ret_reset;
    int ret_deflate_end;
    int bytes_written;

    // step 2: Setup (initialize stream and prepare buffers)
    memset(&strm, 0, sizeof(strm));
    strm.next_in = (Bytef *)inbuf;
    strm.avail_in = in_len;
    strm.next_out = (Bytef *)outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), sizeof(z_stream));

    // step 3: Operate & Validate (set dictionary, compress, write to gzip, retrieve dictionary, reset)
    ret_setdict = deflateSetDictionary(&strm, dictionary, dict_len);
    ret_deflate = deflate(&strm, 0);
    uInt compressed_len = (uInt)strm.total_out;
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    bytes_written = gzwrite(gzf, outbuf, compressed_len);
    newpos = gzseek64(gzf, (off64_t)0, SEEK_SET);
    ret_getdict = deflateGetDictionary(&strm, fetched_dict, &fetched_len);
    ret_reset = deflateReset(&strm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_deflate_init; (void)ret_setdict; (void)ret_deflate; (void)compressed_len; (void)bytes_written; (void)newpos; (void)ret_getdict; (void)fetched_len; (void)ret_reset; (void)ret_deflate_end;
    return 66;
}