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
//<ID> 1643
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *first_ptr = (cJSON *)0;
    cJSON *detached1 = (cJSON *)0;
    cJSON *detached2 = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_item1 = 0;
    cJSON_bool added_item2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "array", arr);
    added_item1 = cJSON_AddItemToArray(arr, item1);
    added_item2 = cJSON_AddItemToArray(arr, item2);

    // step 4: Operate
    first_ptr = cJSON_GetArrayItem(arr, 0);
    detached1 = cJSON_DetachItemViaPointer(arr, first_ptr);
    detached2 = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "moved_one", detached1);
    cJSON_AddItemToObject(root, "moved_two", detached2);
    json_unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_unformatted);

    // step 5: Validate
    validation_score = (int)added_arr
        + (int)added_item1
        + (int)added_item2
        + (int)(json_unformatted != (char *)0)
        + (int)cJSON_IsArray(cJSON_GetObjectItem(root, "array"))
        + (int)cJSON_HasObjectItem(root, "moved_one");

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)validation_score;
    (void)item1;
    (void)item2;
    (void)first_ptr;
    (void)detached1;
    (void)detached2;
    // API sequence test completed successfully
    return 66;
}