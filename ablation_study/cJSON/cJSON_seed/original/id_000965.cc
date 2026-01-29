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
//<ID> 965
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
    cJSON *external = cJSON_CreateObject();
    cJSON_AddNumberToObject(external, "n", 7.0);
    cJSON_bool added_ref = cJSON_AddItemReferenceToArray(arr, external);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddRawToObject(root, "raw", "{\"msg\":\"hello\"}");

    // step 3: Operate
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is", is_raw);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON_PrintPreallocated(root, buffer, 128, 0);
    cJSON_free(buffer);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}