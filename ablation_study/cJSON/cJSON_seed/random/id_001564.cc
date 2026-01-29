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
//<ID> 1564
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
    cJSON *title = (cJSON *)0;
    cJSON *bool_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    char *printed = (char *)0;
    char *title_str = (char *)0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_arr_to_obj = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("first_element");
    title = cJSON_CreateString("collection_title");

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, str_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    bool_ptr = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "title", title);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_title = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(retrieved_title);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_to_array
        + (int)added_arr_to_obj
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(arr_size == 1)
        + (int)(retrieved_title != (cJSON *)0)
        + (int)(title_str != (char *)0)
        + (int)(printed != (char *)0)
        + (int)(bool_ptr != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)arr_size;
    (void)title_str;
    (void)retrieved_arr;
    (void)retrieved_title;
    (void)str_item;
    (void)title;
    // API sequence test completed successfully
    return 66;
}