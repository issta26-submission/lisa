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
//<ID> 1553
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *title_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *printed = (char *)0;
    const char *title_str = (const char *)0;
    cJSON_bool inserted_first = 0;
    cJSON_bool inserted_second = 0;
    cJSON_bool added_arr_to_obj = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    title_ptr = cJSON_AddStringToObject(root, "title", "numbers");

    // step 3: Configure
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(42.0);
    inserted_first = cJSON_InsertItemInArray(arr, 0, num1);
    inserted_second = cJSON_InsertItemInArray(arr, 1, num2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    title_str = cJSON_GetStringValue(title_ptr);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)inserted_first
        + (int)inserted_second
        + (int)added_arr_to_obj
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(first_elem != (cJSON *)0)
        + (int)(title_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)title_str;
    (void)first_elem;
    (void)retrieved_arr;
    (void)num1;
    (void)num2;
    (void)title_ptr;
    // API sequence test completed successfully
    return 66;
}