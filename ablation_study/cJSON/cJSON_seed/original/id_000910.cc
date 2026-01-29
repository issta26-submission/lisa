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
//<ID> 910
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"metadata\":{\"version\":1},\"items\":[{\"id\":100}],\"name\":\"root\"}";
    size_t json_len = (size_t)strlen(json);
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *metadata = cJSON_GetObjectItemCaseSensitive(root, "metadata");
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(metadata, "version");
    double version = cJSON_GetNumberValue(version_item);

    // step 2: Configure
    cJSON *dynamic_array = cJSON_AddArrayToObject(root, "dynamic");
    cJSON *num_entry = cJSON_CreateNumber(7.5);
    cJSON *str_entry = cJSON_CreateString("entry-string");
    cJSON_AddItemToArray(dynamic_array, num_entry);
    cJSON_AddItemToArray(dynamic_array, str_entry);

    // step 3: Operate
    cJSON *detached_items = cJSON_DetachItemFromObject(root, "items");
    cJSON_AddItemToArray(dynamic_array, detached_items);
    cJSON *first_elem = cJSON_GetArrayItem(dynamic_array, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    (void)version;
    (void)first_value;

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    char *prebuf = (char *)buf;
    cJSON_PrintPreallocated(root, prebuf, 256, 1);
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}