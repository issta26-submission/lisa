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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    gzFile gzf;
    Bytef dict[] = { 'e','x','a','m','p','l','e','_','d','i','c','t' };
    uInt dict_len = (uInt)(sizeof(dict));
    unsigned char inbuf[] = "The quick brown fox jumps over the lazy dog";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    unsigned char outbuf[512];
    Bytef getdict[64];
    uInt getdict_len = (uInt)(sizeof(getdict));
    off64_t seek_ret;
    int ret_deflate_init;
    int ret_setdict;
    int ret_deflate;
    int ret_getdict;
    int ret_reset;
    int ret_deflate_end;
    int bytes_written;
    int ret_gzclose;

    // step 2: Setup (initialize stream and set dictionary, open gzip file)
    memset(&strm, 0, sizeof(strm));
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_setdict = deflateSetDictionary(&strm, dict, dict_len);
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "w+b");
    strm.next_in = (Bytef *)inbuf;
    strm.avail_in = in_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Core operations (compress, write, seek, retrieve dictionary, reset)
    ret_deflate = deflate(&strm, 0);
    unsigned int compressed_len = (unsigned int)strm.total_out;
    bytes_written = gzwrite(gzf, outbuf, compressed_len);
    seek_ret = gzseek64(gzf, (off64_t)0, 0);
    ret_getdict = deflateGetDictionary(&strm, getdict, &getdict_len);
    ret_reset = deflateReset(&strm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    ret_gzclose = gzclose_r(gzf);

    (void)ret_deflate_init;
    (void)ret_setdict;
    (void)ret_deflate;
    (void)compressed_len;
    (void)bytes_written;
    (void)seek_ret;
    (void)ret_getdict;
    (void)ret_reset;
    (void)ret_deflate_end;
    (void)ret_gzclose;

    // API sequence test completed successfully
    return 66;
}