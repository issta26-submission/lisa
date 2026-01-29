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
//<ID> 1550
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
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_title = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON_bool inserted_first = 0;
    cJSON_bool inserted_second = 0;
    cJSON_bool added_title = 0;
    cJSON_bool added_arr = 0;
    char *printed = (char *)0;
    const char *title_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(100.0);
    num2 = cJSON_CreateNumber(3.14159);
    str_item = cJSON_CreateString("numbers_list");

    // step 3: Configure
    inserted_first = cJSON_InsertItemInArray(arr, 0, num1);
    inserted_second = cJSON_InsertItemInArray(arr, 1, num2);
    added_title = cJSON_AddItemToObject(root, "title", str_item);
    added_arr = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    retrieved_title = cJSON_GetObjectItem(root, "title");
    title_str = cJSON_GetStringValue(retrieved_title);
    first_elem = cJSON_GetArrayItem(arr, 0);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)inserted_first
        + (int)inserted_second
        + (int)added_title
        + (int)added_arr
        + (int)(title_str != (const char *)0)
        + (int)(first_elem != (cJSON *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_elem;
    (void)retrieved_title;
    (void)num1;
    (void)num2;
    (void)str_item;
    (void)arr;
    // API sequence test completed successfully
    return 66;
}