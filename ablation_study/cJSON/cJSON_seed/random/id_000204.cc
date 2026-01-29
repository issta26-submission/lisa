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
//<ID> 204
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
    cJSON *to_remove = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *retrieved_copy = (cJSON *)0;
    cJSON *count = (cJSON *)0;
    char *json_text = (char *)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    to_remove = cJSON_CreateString("temporary_entry");
    cJSON_AddItemToObject(root, "to_remove", to_remove);
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    retrieved_copy = cJSON_CreateString(cJSON_GetStringValue(retrieved));
    cJSON_AddItemToObject(root, "retrieved_copy", retrieved_copy);
    cJSON_DeleteItemFromObject(root, "to_remove");

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    count = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}