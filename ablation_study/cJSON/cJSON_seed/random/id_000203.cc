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
//<ID> 203
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *to_delete = (cJSON *)0;
    cJSON *picked_item = (cJSON *)0;
    cJSON *picked_copy = (cJSON *)0;
    const char *picked_value = (const char *)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    s1 = cJSON_CreateString("first");
    s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    to_delete = cJSON_CreateString("temporary");
    cJSON_AddItemToObject(root, "delete_me", to_delete);

    // step 4: Operate
    picked_item = cJSON_GetArrayItem(arr, 1);
    picked_value = cJSON_GetStringValue(picked_item);
    picked_copy = cJSON_CreateString(picked_value ? picked_value : "");
    cJSON_AddItemToObject(root, "picked", picked_copy);
    cJSON_DeleteItemFromObject(root, "delete_me");

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}