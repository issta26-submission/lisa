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
//<ID> 1541
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
    cJSON *raw_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *title_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON_bool added_raw = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("raw_payload");
    str_item = cJSON_CreateString("text_element");

    // step 3: Configure
    added_raw = cJSON_AddItemToArray(arr, raw_item);
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_obj = cJSON_AddItemToObject(root, "list", arr);
    title_ptr = cJSON_AddStringToObject(root, "title", "example_title");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    is_raw = cJSON_IsRaw(first_elem);

    // step 5: Validate
    validation_score = (int)added_raw + (int)added_str + (int)added_obj + (int)(title_ptr != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(first_elem != (cJSON *)0) + (int)is_raw;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)title_ptr;
    (void)retrieved_arr;
    (void)first_elem;
    (void)raw_item;
    (void)str_item;
    // API sequence test completed successfully
    return 66;
}