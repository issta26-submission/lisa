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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char dict_c[] = "example dictionary for inflate";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char src_c[] = "Sample data to be compressed and used with inflate dictionary";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = new Bytef[bound];
    uLongf compLen = (uLongf)bound;
    Bytef outbuf[1024];
    const char *ver = zlibVersion();

    // step 2: Setup -> Compress & Init inflater
    int rc_compress = compress2(comp, &compLen, source, sourceLen, 6);
    int rc_inflateInit = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 3: Configure -> Set dictionary & Validate; record status to gzip
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    int rc_setdict = inflateSetDictionary(&strm, dictionary, dictLen);
    int rc_validate = inflateValidate(&strm, 15);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int errnum = 0;
    const char *errstr = gzerror(gz, &errnum);
    gzprintf(gz, "ver=%s rc_compress=%d compLen=%lu rc_inflateInit=%d rc_setdict=%d rc_validate=%d errnum=%d errstr=%s\n",
             ver, rc_compress, (unsigned long)compLen, rc_inflateInit, rc_setdict, rc_validate, errnum, errstr);

    // step 4: Cleanup
    inflateEnd(&strm);
    int rc_gzclose = gzclose(gz);
    delete [] comp;
    (void)rc_compress; (void)rc_inflateInit; (void)rc_setdict; (void)rc_validate; (void)rc_gzclose; (void)errstr;

    // API sequence test completed successfully
    return 66;
}