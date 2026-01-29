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
//<ID> 541
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *list = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *first_in_list = (cJSON *)0;
    const char *first_str = (const char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_list_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    list = cJSON_CreateArray();
    str_item = cJSON_CreateString("entry-0");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(list, str_item);
    add_ok2 = cJSON_AddItemToObject(root, "list", list);
    add_ok3 = cJSON_AddItemToObject(root, "missing", null_item);

    // step 4: Operate
    retrieved_null = cJSON_GetObjectItem(root, "missing");
    detached_item = cJSON_DetachItemViaPointer(root, retrieved_null);
    retrieved_list = cJSON_GetObjectItem(root, "list");
    is_list_array = cJSON_IsArray(retrieved_list);
    first_in_list = cJSON_GetArrayItem(retrieved_list, 0);
    first_str = cJSON_GetStringValue(first_in_list);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)is_list_array;
    (void)first_str;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_item);

    // API sequence test completed successfully
    return 66;
}