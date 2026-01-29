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
//<ID> 1049
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
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON *flag_true = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_SetValuestring(name, "modified_name");
    cJSON_AddItemToObjectCS(root, "name", name);
    cJSON_AddItemToObjectCS(root, "values", arr);
    cJSON_AddItemToObjectCS(root, "enabled", flag_true);
    cJSON *is_array_flag = cJSON_CreateBool(cJSON_IsArray(arr));
    cJSON_AddItemToObjectCS(root, "values_is_array", is_array_flag);

    // step 3: Operate
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30));
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    char *prealloc_buf = (char *)cJSON_malloc(256);
    memset(prealloc_buf, 0, 256);
    cJSON_PrintPreallocated(root, prealloc_buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}