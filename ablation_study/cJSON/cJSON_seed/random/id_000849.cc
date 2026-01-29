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
//<ID> 849
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
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *retrieved_a = (cJSON *)0;
    cJSON *retrieved_b = (cJSON *)0;
    char *printed = (char *)0;
    double value_a = 0.0;
    double value_b = 0.0;
    int validation_score = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_array = 0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    num_a = cJSON_CreateNumber(7.5);
    num_b = cJSON_CreateNumber(7.5);

    // step 3: Configure
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_array = cJSON_AddItemToObject(root, "items", arr);
    added_a = cJSON_AddItemToObject(root, "alpha", num_a);
    added_b = cJSON_AddItemToObject(root, "beta", num_b);

    // step 4: Operate
    retrieved_a = cJSON_GetObjectItem(root, "alpha");
    retrieved_b = cJSON_GetObjectItem(root, "beta");
    value_a = cJSON_GetNumberValue(retrieved_a);
    value_b = cJSON_GetNumberValue(retrieved_b);
    printed = cJSON_Print(root);

    // step 5: Validate
    validation_score = (int)added_str + (int)added_array + (int)added_a + (int)added_b + (printed != (char *)0) + (retrieved_a != (cJSON *)0) + (retrieved_b != (cJSON *)0) + (value_a == value_b);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}