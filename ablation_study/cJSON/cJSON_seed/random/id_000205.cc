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
//<ID> 205
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
    cJSON *item2 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *copied = (cJSON *)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item0 = cJSON_CreateString("first");
    item1 = cJSON_CreateString("second_to_copy");
    item2 = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    copied = cJSON_CreateString(retrieved->valuestring);
    cJSON_AddItemToObject(root, "copied_from_array", copied);
    cJSON_AddItemToObject(root, "will_be_deleted", cJSON_CreateString("temporary_value"));

    // step 5: Validate
    cJSON_DeleteItemFromObject(root, "will_be_deleted");
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}