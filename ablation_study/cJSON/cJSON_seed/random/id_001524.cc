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
//<ID> 1524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;
    double count_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);
    flag = cJSON_CreateFalse();

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(array, num1);
    added_num2 = cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    retrieved_count = cJSON_GetObjectItem(root, "count");
    is_false = cJSON_IsFalse(retrieved_flag);
    count_value = cJSON_GetNumberValue(retrieved_count);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_num1 + (int)added_num2 + (int)(retrieved_flag != (cJSON *)0) + (int)is_false + (int)(count_value == 2.0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_count;
    (void)retrieved_flag;
    (void)count_value;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}