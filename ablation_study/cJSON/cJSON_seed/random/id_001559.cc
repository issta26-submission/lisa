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
//<ID> 1559
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
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *title = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    char *printed = (char *)0;
    const char *title_str = (const char *)0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_title_to_obj = 0;
    cJSON_bool added_num_a_to_arr = 0;
    cJSON_bool inserted_num_b_into_arr = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(3.14);
    num_b = cJSON_CreateNumber(42.0);
    title = cJSON_CreateString("numeric collection");

    // step 3: Configure
    added_num_a_to_arr = cJSON_AddItemToArray(arr, num_a);
    inserted_num_b_into_arr = cJSON_InsertItemInArray(arr, 0, num_b);
    added_arr_to_obj = cJSON_AddItemToObject(root, "numbers", arr);
    added_title_to_obj = cJSON_AddItemToObject(root, "title", title);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_title = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(retrieved_title);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_num_a_to_arr
        + (int)inserted_num_b_into_arr
        + (int)added_arr_to_obj
        + (int)added_title_to_obj
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(first_elem != (cJSON *)0)
        + (int)(retrieved_title != (cJSON *)0)
        + (int)(title_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_elem;
    (void)retrieved_arr;
    (void)retrieved_title;
    (void)title_str;
    (void)num_a;
    (void)num_b;
    (void)title;
    // API sequence test completed successfully
    return 66;
}