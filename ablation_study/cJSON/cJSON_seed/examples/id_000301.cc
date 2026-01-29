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
//<ID> 301
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
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *null_item = cJSON_CreateNull();
    cJSON *label = cJSON_CreateString("sample_label");
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc((size_t)256);
    memset(buffer, 0, (size_t)256);
    cJSON_AddItemToObject(root, "printed_snapshot", cJSON_CreateString(printed));

    // step 3: Operate & Validate
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second_elem);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON_AddItemToObject(root, "second_is_null", cJSON_CreateBool(second_is_null));
    cJSON_AddItemToObject(root, "has_data", cJSON_CreateBool(has_data));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}