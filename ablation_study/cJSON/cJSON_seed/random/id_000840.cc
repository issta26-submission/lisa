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
//<ID> 840
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
    cJSON *label = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    double extracted_value = 0.0;
    cJSON_bool add1 = 0;
    cJSON_bool add2 = 0;
    cJSON_bool added_arr = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14159);
    num2 = cJSON_CreateNumber(2.71828);
    label = cJSON_CreateString("constants");

    // step 3: Configure
    add1 = cJSON_AddItemToArray(arr, num1);
    add2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_num = cJSON_GetArrayItem(retrieved_arr, 0);
    extracted_value = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (printed != (char *)0) + (retrieved_arr != (cJSON *)0) + (retrieved_num != (cJSON *)0) + (add1 != 0) + (add2 != 0) + (added_arr != 0) + (extracted_value > 0.0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}