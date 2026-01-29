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
//<ID> 954
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"items\": [{\"name\":\"alpha\"}], \"count\": 1}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(new_obj, "name", "beta");
    cJSON_InsertItemInArray(items, 1, new_obj);
    cJSON *str_ref = cJSON_CreateStringReference("gamma");
    cJSON_InsertItemInArray(items, 2, str_ref);
    cJSON *extras = cJSON_AddObjectToObject(root, "extras");
    cJSON_AddNumberToObject(extras, "added", 42.0);

    // step 3: Operate
    cJSON *items_ref = cJSON_CreateArrayReference(items);
    cJSON_AddItemReferenceToObject(extras, "items_ref", items_ref);
    char *out = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}