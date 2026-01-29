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
//<ID> 1522
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
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_external = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;
    double ext_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.0);
    num2 = cJSON_CreateNumber(4.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", arr);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    retrieved_external = cJSON_AddNumberToObject(root, "external", 99.0);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_external = cJSON_GetObjectItem(root, "external");
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_flag);
    ext_value = cJSON_GetNumberValue(retrieved_external);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_num1 + (int)added_num2 + (int)(retrieved_external != (cJSON *)0) + (int)is_false + (int)(ext_value == 99.0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)ext_value;
    (void)retrieved_flag;
    (void)retrieved_external;
    // API sequence test completed successfully
    return 66;
}