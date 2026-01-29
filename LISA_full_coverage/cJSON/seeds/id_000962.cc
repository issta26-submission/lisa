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
//<ID> 962
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    const char raw_payload[] = "{\"raw_key\":123}";
    cJSON *raw_item = cJSON_AddRawToObject(root, "rawdata", raw_payload);
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", is_raw);

    // step 3: Operate
    cJSON_AddItemReferenceToArray(arr, raw_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "temp_false", false_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp_false");

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}