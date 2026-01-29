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
//<ID> 1558
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON_bool inserted0 = 0;
    cJSON_bool inserted1 = 0;
    cJSON_bool added_arr = 0;
    const char *title_val = (const char *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    double value0 = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14159);
    num1 = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("numbers_list");

    // step 3: Configure
    inserted0 = cJSON_InsertItemInArray(arr, 0, num0);
    inserted1 = cJSON_InsertItemInArray(arr, 1, num1);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "title", str_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_str = cJSON_GetObjectItem(root, "title");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    value0 = cJSON_GetNumberValue(elem0);
    title_val = cJSON_GetStringValue(retrieved_str);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)inserted0
        + (int)inserted1
        + (int)added_arr
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(retrieved_str != (cJSON *)0)
        + (int)(elem0 != (cJSON *)0)
        + (int)(title_val != (const char *)0)
        + (int)(printed != (char *)0)
        + (int)(value0 > 0.0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)value0;
    (void)title_val;
    (void)retrieved_arr;
    (void)retrieved_str;
    (void)elem0;
    (void)num0;
    (void)num1;
    (void)str_item;
    // API sequence test completed successfully
    return 66;
}