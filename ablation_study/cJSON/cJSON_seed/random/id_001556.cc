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
//<ID> 1556
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
    cJSON *name_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool inserted_num = 0;
    const char *name_str = (const char *)0;
    char *printed = (char *)0;
    int validation_score = 0;
    double num_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("tester");
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    inserted_num = cJSON_InsertItemInArray(arr, 0, num_item);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(retrieved_name);
    retrieved_num = cJSON_GetArrayItem(arr, 0);
    num_value = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_name
        + (int)added_arr
        + (int)inserted_num
        + (int)(name_str != (const char *)0)
        + (int)(retrieved_num != (cJSON *)0)
        + (int)(printed != (char *)0)
        + (int)(num_value == 42.0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)name_str;
    (void)retrieved_name;
    (void)retrieved_num;
    (void)num_value;
    // API sequence test completed successfully
    return 66;
}