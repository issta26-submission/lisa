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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "node");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *flag = cJSON_AddTrueToObject(root, "ok");

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(list, n1);
    cJSON_AddItemToArray(list, n2);

    // step 3: Operate and Validate
    cJSON *list_lookup = cJSON_GetObjectItem(root, "list");
    cJSON_bool is_arr = cJSON_IsArray(list_lookup);
    cJSON *detached_child = cJSON_DetachItemViaPointer(root, child);
    char *printed_root = cJSON_Print(root);
    char *printed_detached = cJSON_Print(detached_child);
    int summary = (int)is_arr + (int)printed_root[0] + (int)printed_detached[0];
    (void)summary;
    (void)flag;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached_child);
    cJSON_Delete(root);

    return 66; // API sequence test completed successfully
}