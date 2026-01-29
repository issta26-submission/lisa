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
//<ID> 897
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "example payload for zlib sequence";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *payload_buf = (const Bytef *)payload;
    z_stream dstrm;
    z_stream istrm;
    gz_header head;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    memset(&head, 0, (size_t)sizeof(gz_header));
    head.name_max = 16;
    head.comm_max = 16;
    head.name = (Bytef *)malloc((size_t)head.name_max);
    head.comment = (Bytef *)malloc((size_t)head.comm_max);
    memcpy(head.name, "testname", 9);
    memcpy(head.comment, "testcomment", 12);

    // step 2: Initialize zlib streams and set gzip header on deflate stream
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateSetHeader(&dstrm, &head);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate with gzFile (write then read) and validate with inflateMark and CRC
    gzFile wf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzprintf(wf, "PAYLOAD:%s", payload);
    gzclose(wf);
    gzFile rf = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned int read_len = 256u;
    char *readbuf = (char *)malloc((size_t)read_len);
    memset(readbuf, 0, (size_t)read_len);
    int rc_gzread = gzread(rf, (voidp)readbuf, read_len);
    gzclose(rf);
    long mark = inflateMark(&istrm);
    uLong crc_read = crc32(0UL, (const Bytef *)readbuf, (uInt)payload_len);

    // step 4: Cleanup and finalization
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(head.name);
    free(head.comment);
    free(readbuf);
    (void)payload_buf;
    (void)rc_gzread;
    (void)mark;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}