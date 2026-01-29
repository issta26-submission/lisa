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
//<ID> 1711
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "name", "fuzz");
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 2: Configure
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToArray(flags, cJSON_CreateFalse());
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());

    // step 3: Operate and Validate
    cJSON *retr_flags = cJSON_GetObjectItem(root, "flags");
    cJSON_bool is_arr = cJSON_IsArray(retr_flags);
    int arr_size = cJSON_GetArraySize(retr_flags);
    char *prebuf = (char*)cJSON_malloc((size_t)512);
    memset(prebuf, 0, 512);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 1);
    char *unformatted = cJSON_PrintUnformatted(root);
    char marker[4];
    memset(marker, 0, sizeof(marker));
    marker[0] = prebuf[0];
    marker[1] = (char)('0' + (int)is_arr);
    marker[2] = (char)('0' + (arr_size % 10));
    (void)pre_ok;
    (void)marker;

    // step 4: Cleanup
    cJSON_free(prebuf);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}