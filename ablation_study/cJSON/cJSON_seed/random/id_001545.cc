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
//<ID> 1545
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
    cJSON *str_item = (cJSON *)0;
    cJSON *added_title_ptr = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    char *printed = (char *)0;
    const char *elem_str = (const char *)0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_to_array = 0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("alpha");

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, str_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    added_title_ptr = cJSON_AddStringToObject(root, "title", "collection");

    // step 4: Operate
    retrieved_elem = cJSON_GetArrayItem(arr, 0);
    elem_str = cJSON_GetStringValue(retrieved_elem);
    is_raw = cJSON_IsRaw(retrieved_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_to_array
        + (int)added_arr_to_obj
        + (int)(added_title_ptr != (cJSON *)0)
        + (int)(retrieved_elem != (cJSON *)0)
        + (int)(elem_str != (const char *)0)
        + (int)is_raw
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)elem_str;
    (void)retrieved_elem;
    (void)added_title_ptr;
    // API sequence test completed successfully
    return 66;
}