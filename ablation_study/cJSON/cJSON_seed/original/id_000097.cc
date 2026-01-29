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
//<ID> 97
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
    cJSON_AddStringToObject(meta, "name", "fuzzmeta");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag = cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *str1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, str1);

    // step 3: Operate and Validate
    cJSON_bool flag_true = cJSON_IsTrue(flag);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *printed = cJSON_Print(root);
    int summary = (int)flag_true + (int)root_is_obj + (int)prealloc_ok + (int)(printed ? printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}