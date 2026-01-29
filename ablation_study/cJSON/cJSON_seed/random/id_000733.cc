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
//<ID> 733
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
    cJSON *item_str = (cJSON *)0;
    cJSON *item_num = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *got_before = (cJSON *)0;
    cJSON *got_after = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_item1 = 0;
    cJSON_bool added_item2 = 0;
    cJSON_bool added_name = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_str = cJSON_CreateString("one");
    item_num = cJSON_CreateNumber(2.0);
    name_item = cJSON_CreateString("tester");

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "items", arr);
    added_item1 = cJSON_AddItemToArray(arr, item_str);
    added_item2 = cJSON_AddItemToArray(arr, item_num);
    added_name = cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate
    got_before = cJSON_GetObjectItem(root, "name");
    detached = cJSON_DetachItemFromObject(root, "name");
    got_after = cJSON_GetObjectItem(root, "name");
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (printed != (char *)0)
                     + (int)added_arr + (int)added_item1 + (int)added_item2 + (int)added_name
                     + (got_before != (cJSON *)0) + (got_after == (cJSON *)0)
                     + (cJSON_GetArraySize(arr) == 2);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}