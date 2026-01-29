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
//<ID> 967
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"array\": [], \"obj\": {\"initial\": 123}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *array = cJSON_GetObjectItem(root, "array");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(obj, "flag", false_item);
    cJSON_AddItemReferenceToArray(array, false_item);
    cJSON_AddRawToObject(root, "raw_blob", "{\"inner\": [1,2,3]}");
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw_blob");
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_check);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "obj");

    // step 3: Operate
    char *unformatted = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}