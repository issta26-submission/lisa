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
//<ID> 1555
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
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *title_item = (cJSON *)0;
    cJSON *first_array_elem = (cJSON *)0;
    cJSON_bool inserted_first = 0;
    cJSON_bool inserted_second = 0;
    cJSON_bool added_arr_to_obj = 0;
    char *printed = (char *)0;
    const char *title_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item1 = cJSON_CreateNumber(42.0);
    num_item2 = cJSON_CreateNumber(3.14);
    title_item = cJSON_CreateString("numbers_list");

    // step 3: Configure
    inserted_first = cJSON_InsertItemInArray(arr, 0, num_item1);
    inserted_second = cJSON_InsertItemInArray(arr, 1, num_item2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "title", title_item);

    // step 4: Operate
    first_array_elem = cJSON_GetArrayItem(arr, 0);
    title_str = cJSON_GetStringValue(title_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)inserted_first
        + (int)inserted_second
        + (int)added_arr_to_obj
        + (int)(first_array_elem != (cJSON *)0)
        + (int)(title_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_array_elem;
    (void)title_str;
    (void)num_item1;
    (void)num_item2;
    (void)inserted_first;
    (void)inserted_second;
    (void)added_arr_to_obj;
    // API sequence test completed successfully
    return 66;
}