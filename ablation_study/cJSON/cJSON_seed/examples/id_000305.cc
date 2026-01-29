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
//<ID> 305
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *raw_blob = cJSON_CreateRaw("{\"inlined\":true}");
    cJSON_AddItemToObject(root, "raw_blob", raw_blob);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double second_is_null_flag = (double)cJSON_IsNull(second);
    cJSON_AddNumberToObject(root, "second_is_null", second_is_null_flag);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON_AddNumberToObject(root, "has_data", (double)has_data);
    int bufsize = 256;
    char *buffer = (char *)cJSON_malloc((size_t)bufsize);
    memset(buffer, 0, (size_t)bufsize);
    cJSON_PrintPreallocated(root, buffer, bufsize, (cJSON_bool)1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}