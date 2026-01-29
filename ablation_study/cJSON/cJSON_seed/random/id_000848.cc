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
//<ID> 848
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
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    double num_val = 0.0;
    cJSON_bool added_str = 0;
    cJSON_bool added_num = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("hello");
    num_item = cJSON_CreateNumber(7.5);
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_num = cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    retrieved_num = cJSON_GetArrayItem(retrieved_arr, 1);
    num_val = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (retrieved_arr != (cJSON *)0) + (retrieved_num != (cJSON *)0) + (printed != (char *)0) + (int)added_str + (int)added_num + (num_val == 7.5);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}