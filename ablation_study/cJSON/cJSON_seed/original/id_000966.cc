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
//<ID> 966
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\": {\"items\": [], \"extras\": {}}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *items = cJSON_GetObjectItem(root_obj, "items");
    cJSON *extras = cJSON_GetObjectItem(root_obj, "extras");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root_obj, "flag", false_item);
    cJSON_AddItemReferenceToArray(items, false_item);
    cJSON *temp_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(extras, "temp", temp_false);
    cJSON *raw_node = cJSON_AddRawToObject(extras, "rawdata", "\"raw_text\"");

    // step 3: Operate
    cJSON_bool raw_flag = cJSON_IsRaw(raw_node);
    cJSON *num_node = cJSON_CreateNumber((double)raw_flag);
    cJSON_AddItemToObject(root_obj, "is_raw_numeric", num_node);
    double set_result = cJSON_SetNumberHelper(num_node, 123.0 + (double)raw_flag);
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(extras, "temp");
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}