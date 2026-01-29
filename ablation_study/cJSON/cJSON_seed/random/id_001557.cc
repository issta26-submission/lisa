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
//<ID> 1557
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
    cJSON *title = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool inserted_num2 = 0;
    cJSON_bool added_arr_to_obj = 0;
    char *printed = (char *)0;
    const char *title_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(1.618);
    title = cJSON_CreateString("values");

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(arr, num1);
    inserted_num2 = cJSON_InsertItemInArray(arr, 0, num2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "title", title);

    // step 4: Operate
    retrieved_array = cJSON_GetObjectItem(root, "values");
    second_elem = cJSON_GetArrayItem(retrieved_array, 1);
    retrieved_title = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(retrieved_title);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_num1
        + (int)inserted_num2
        + (int)added_arr_to_obj
        + (int)(second_elem != (cJSON *)0)
        + (int)(title_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_array;
    (void)second_elem;
    (void)retrieved_title;
    (void)num1;
    (void)num2;
    (void)title;
    // API sequence test completed successfully
    return 66;
}