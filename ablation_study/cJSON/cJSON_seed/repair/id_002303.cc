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
//<ID> 2303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "type", "collection");
    cJSON_AddStringToObject(meta, "name", "device-123");

    // step 2: Configure
    cJSON_AddItemToArray(items, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON *subarr = cJSON_CreateArray();
    cJSON_AddItemToArray(subarr, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(items, subarr);

    // step 3: Operate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_type = cJSON_GetObjectItem(got_meta, "type");
    cJSON_bool type_is_string = cJSON_IsString(got_type);
    char *type_value = cJSON_GetStringValue(got_type);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_AddStringToObject(root, "extracted_type", type_value ? type_value : "");
    cJSON_AddItemToObject(root, "items_is_array", cJSON_CreateBool(items_is_array));

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = items_is_array ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}