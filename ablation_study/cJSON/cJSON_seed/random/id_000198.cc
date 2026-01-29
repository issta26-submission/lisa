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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    char *first_value = (char *)0;
    cJSON *first_flag = (cJSON *)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Configure
    null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 0);

    // step 5: Validate
    first = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetStringValue(first);
    first_flag = cJSON_CreateString(first_value ? first_value : ""); 
    cJSON_AddItemToObject(root, "first_value", first_flag);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}