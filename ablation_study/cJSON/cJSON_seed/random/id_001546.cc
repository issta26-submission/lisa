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
//<ID> 1546
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    cJSON *title_ptr = (cJSON *)0;
    cJSON *added_title_ptr = (cJSON *)0;
    cJSON_bool added_elem0 = 0;
    cJSON_bool added_elem1 = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_raw_to_obj = 0;
    cJSON_bool raw_check = 0;
    const char *elem_str = (const char *)0;
    const char *title_str = (const char *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("first");
    elem1 = cJSON_CreateString("second");
    raw_item = cJSON_CreateRaw("raw-data");

    // step 3: Configure
    added_elem0 = cJSON_AddItemToArray(arr, elem0);
    added_elem1 = cJSON_AddItemToArray(arr, elem1);
    added_arr_to_obj = cJSON_AddItemToObject(root, "list", arr);
    added_raw_to_obj = cJSON_AddItemToObject(root, "raw", raw_item);
    added_title_ptr = cJSON_AddStringToObject(root, "title", "demo");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    retrieved_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    elem_str = cJSON_GetStringValue(retrieved_elem);
    title_ptr = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(title_ptr);
    raw_check = cJSON_IsRaw(cJSON_GetObjectItem(root, "raw"));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_elem0
        + (int)added_elem1
        + (int)added_arr_to_obj
        + (int)added_raw_to_obj
        + (int)(added_title_ptr != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(retrieved_elem != (cJSON *)0)
        + (int)(elem_str != (const char *)0)
        + (int)(title_str != (const char *)0)
        + (int)raw_check
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)elem_str;
    (void)title_str;
    (void)retrieved_arr;
    (void)retrieved_elem;
    (void)title_ptr;
    (void)added_title_ptr;
    // API sequence test completed successfully
    return 66;
}