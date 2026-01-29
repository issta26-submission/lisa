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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNull();
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(data, n0);
    cJSON_AddItemToArray(data, n1);
    cJSON_AddItemToArray(data, s1);
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    const char *raw_text = "{\"raw\":true}";
    cJSON *raw_item = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(meta, "raw_blob", raw_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *has_data_item = cJSON_CreateBool(has_data);
    cJSON_AddItemToObject(root, "has_data", has_data_item);
    cJSON *first = cJSON_GetArrayItem(data, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    cJSON *first_is_null_item = cJSON_CreateBool(first_is_null);
    cJSON_AddItemToObject(root, "first_is_null", first_is_null_item);
    char *prebuf = (char*)cJSON_malloc((size_t)256);
    memset(prebuf, 0, 256);
    cJSON_PrintPreallocated(root, prebuf, 256, (cJSON_bool)1);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_AddItemToObject(root, "printed_unformatted", cJSON_CreateString(printed));

    // step 4: Cleanup
    cJSON_free(prebuf);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}