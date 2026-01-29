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
//<ID> 1552
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
    cJSON *title_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON_bool inserted0 = 0;
    cJSON_bool inserted1 = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_title = 0;
    char *printed = (char *)0;
    const char *title_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.1415);
    num2 = cJSON_CreateNumber(2.7182);
    title_item = cJSON_CreateString("numeric_list");

    // step 3: Configure
    inserted0 = cJSON_InsertItemInArray(arr, 0, num1);
    inserted1 = cJSON_InsertItemInArray(arr, 1, num2);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);
    added_title = cJSON_AddItemToObject(root, "title", title_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_title = cJSON_GetObjectItem(root, "title");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    title_str = cJSON_GetStringValue(retrieved_title);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)inserted0
        + (int)inserted1
        + (int)added_arr_to_obj
        + (int)added_title
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(retrieved_title != (cJSON *)0)
        + (int)(elem0 != (cJSON *)0)
        + (int)(elem1 != (cJSON *)0)
        + (int)(title_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)title_str;
    (void)elem0;
    (void)elem1;
    (void)retrieved_arr;
    (void)retrieved_title;
    (void)num1;
    (void)num2;
    (void)title_item;
    (void)arr;
    (void)added_arr_to_obj;
    (void)added_title;
    // API sequence test completed successfully
    return 66;
}