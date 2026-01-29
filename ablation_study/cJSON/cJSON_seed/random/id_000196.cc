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
//<ID> 196
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    char *json_text = (char *)0;
    int size_after = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateNumber(42.0);
    item1 = cJSON_CreateNumber(100.0);
    null_item = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, null_item);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size_after_delete", (double)size_after);

    // step 5: Validate
    first = cJSON_GetArrayItem(arr, 0);
    cJSON_AddItemToObject(root, "first_is_number", cJSON_CreateBool(cJSON_IsNumber(first)));
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}