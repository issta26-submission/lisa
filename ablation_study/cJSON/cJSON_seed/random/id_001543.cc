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
//<ID> 1543
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
    cJSON *str_item1 = (cJSON *)0;
    cJSON *str_item2 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *title_item = (cJSON *)0;
    cJSON_bool added_first = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool is_raw = 0;
    char *printed = (char *)0;
    const char *elem_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item1 = cJSON_CreateString("alpha");
    str_item2 = cJSON_CreateString("beta");

    // step 3: Configure
    added_first = cJSON_AddItemToArray(arr, str_item1);
    cJSON_AddItemToArray(arr, str_item2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    title_item = cJSON_AddStringToObject(root, "title", "sequence");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem_str = cJSON_GetStringValue(elem0);
    is_raw = cJSON_IsRaw(elem0);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_first
        + (int)added_arr_to_obj
        + (int)(title_item != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(elem0 != (cJSON *)0)
        + (int)(elem_str != (const char *)0)
        + (int)is_raw
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)elem_str;
    (void)is_raw;
    // API sequence test completed successfully
    return 66;
}