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
//<ID> 916
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

    // step 2: Configure
    cJSON *elem_bool = cJSON_CreateBool(0);
    cJSON_AddItemToArray(list, elem_bool);
    cJSON *elem_str = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(list, elem_str);
    cJSON *elem_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(list, elem_num);
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateBool(1);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", replacement);
    cJSON *detached_list = cJSON_DetachItemFromObject(root, "list");
    int count = cJSON_GetArraySize(detached_list);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddNumberToObject(root, "replaced_flag", (double)replaced);

    // step 4: Cleanup
    cJSON_Delete(detached_list);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}