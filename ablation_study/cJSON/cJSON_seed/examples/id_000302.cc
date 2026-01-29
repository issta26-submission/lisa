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
//<ID> 302
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");

    // step 2: Configure
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    cJSON *first_is_null_item = cJSON_CreateBool(first_is_null);
    cJSON_AddItemToObject(root, "first_is_null", first_is_null_item);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *has_data_item = cJSON_CreateBool(has_data);
    cJSON_AddItemToObject(root, "has_data", has_data_item);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    void *buf_void = cJSON_malloc((size_t)128);
    char *buf = (char *)buf_void;
    memset(buf, 0, 128);
    cJSON_PrintPreallocated(root, buf, (int)128, (cJSON_bool)1);

    // step 4: Cleanup
    cJSON_free(buf_void);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}