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
//<ID> 834
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char init_json[] = "{\"items\":[10,20],\"flag\":false}";
    size_t init_len = sizeof(init_json) - 1;
    cJSON *root = cJSON_ParseWithLength(init_json, init_len);

    // step 2: Configure
    cJSON *added_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "added_array", added_arr);
    cJSON *n1 = cJSON_CreateNumber(100.5);
    cJSON *n2 = cJSON_CreateNumber(200.25);
    cJSON_AddItemToArray(added_arr, n1);
    cJSON_AddItemToArray(added_arr, n2);
    cJSON_AddRawToObject(root, "raw_blob", "{\"nested\":true,\"value\":123}");
    cJSON_AddBoolToObject(root, "newflag", 1);

    // step 3: Operate
    cJSON *items_obj = cJSON_GetObjectItem(root, "items");
    cJSON_bool added_is_array = cJSON_IsArray(cJSON_GetObjectItem(root, "added_array"));
    int items_size = cJSON_GetArraySize(items_obj);
    cJSON *first_item = cJSON_GetArrayItem(items_obj, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON *got_raw = cJSON_GetObjectItem(root, "raw_blob");
    cJSON_bool raw_is_raw = cJSON_IsRaw(got_raw);
    (void)added_is_array;
    (void)items_size;
    (void)first_value;
    (void)raw_is_raw;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}