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
//<ID> 853
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong DATA_LEN = 1024UL;
    Bytef *input = (Bytef *)malloc((size_t)DATA_LEN);
    memset(input, 'A', (size_t)DATA_LEN);
    uLong adl = adler32(1UL, input, (uInt)DATA_LEN);

    // step 2: Initialize streams (deflate for lifecycle demonstration, inflate for operation)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int windowBits = 15;
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = input;
    istrm.avail_in = (uInt)DATA_LEN;
    istrm.adler = adl;
    Bytef *out = (Bytef *)malloc((size_t)DATA_LEN);
    memset(out, 0, (size_t)DATA_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)DATA_LEN;

    // step 3: Core operations — run inflate on provided buffer and exercise gzread by creating a gz file and reading it
    int inflate_status = inflate(&istrm, 0);
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gw, input, (unsigned int)DATA_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef *gzbuf = (Bytef *)malloc((size_t)DATA_LEN);
    memset(gzbuf, 0, (size_t)DATA_LEN);
    int read_n = gzread(gr, (voidp)gzbuf, (unsigned int)DATA_LEN);
    gzclose(gr);

    // step 4: Cleanup and finalize
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(input);
    free(out);
    free(gzbuf);
    (void)ver;
    (void)adl;
    (void)inflate_status;
    (void)read_n;
    // API sequence test completed successfully
    return 66;
}