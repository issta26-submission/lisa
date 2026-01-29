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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("elem");
    cJSON_AddItemToArray(arr, str);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToArray(arr, meta_dup);

    // step 3: Operate and Validate
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    char *printed = cJSON_Print(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool preallocated_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    int summary = (int)meta_is_obj + (int)flag_is_true + (printed ? (int)printed[0] : 0) + (preallocated_ok ? (int)buffer[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}