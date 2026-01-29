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
//<ID> 300
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
    cJSON *raw = cJSON_CreateRaw("{\"inline\":42}");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *has_data_item = cJSON_CreateBool(has_data);
    cJSON_AddItemToObject(root, "has_data", has_data_item);

    // step 3: Operate & Validate
    cJSON *got_arr = cJSON_GetObjectItem(root, "data");
    cJSON *first_item = cJSON_GetArrayItem(got_arr, 0);
    cJSON *second_item = cJSON_GetArrayItem(got_arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second_item);
    cJSON *second_is_null_item = cJSON_CreateBool(second_is_null);
    cJSON_AddItemToObject(root, "second_is_null", second_is_null_item);
    char *prebuf = (char *)cJSON_malloc(128);
    memset(prebuf, 0, 128);
    cJSON_PrintPreallocated(root, prebuf, (int)128, (cJSON_bool)1);
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_AddItemToObject(root, "unformatted", cJSON_CreateString(unformatted));

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}