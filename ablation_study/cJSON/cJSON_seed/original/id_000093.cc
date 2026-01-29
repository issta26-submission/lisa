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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "name", "fuzz");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active = cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool active_is_true = cJSON_IsTrue(active);
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *alloc_print = cJSON_Print(root);
    int summary = (int)root_is_obj + (int)active_is_true + (int)buffer[0] + (int)alloc_print[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(alloc_print);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}