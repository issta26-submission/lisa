#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *t1 = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, t1);
    cJSON *t2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, t2);
    cJSON *t3 = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, t3);
    cJSON *null_item = cJSON_AddNullToObject(meta, "optional");

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    int flags_count = cJSON_GetArraySize(flags);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = (char)('0' + (flags_count % 10));
    buffer[2] = buffer[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}