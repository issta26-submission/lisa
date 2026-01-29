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
//<ID> 1305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload with dictionary";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char dict_data[] = "preset_dictionary";
    const uInt dictLen = (uInt)(sizeof(dict_data) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    Bytef *dictBuf = new Bytef[(size_t)dictLen];
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    uLong decompBound = srcLen + 64;
    Bytef *decompBuf = new Bytef[(size_t)decompBound];
    memset(src, 0, (size_t)srcLen);
    memset(dictBuf, 0, (size_t)dictLen);
    memset(compBuf, 0, (size_t)bound);
    memset(decompBuf, 0, (size_t)decompBound);
    memcpy(src, payload, (size_t)srcLen);
    memcpy(dictBuf, dict_data, (size_t)dictLen);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate with a preset dictionary)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dictBuf, dictLen);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    uLong compLen = bound - (uLong)dstrm.avail_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Core operations (open gzip, write compressed data, combine checksums, inflate)
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    uLong ad1 = adler32(0UL, dictBuf, dictLen);
    uLong ad2 = adler32(0UL, compBuf, (uInt)compLen);
    uLong combined = adler32_combine(ad1, ad2, (off_t)compLen);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)decompBound;
    int rc_inflate = inflate(&istrm, Z_FINISH);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] dictBuf;
    delete [] compBuf;
    delete [] decompBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}