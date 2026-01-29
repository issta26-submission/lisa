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
//<ID> 1527
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_array = 0;
    cJSON_bool added_n1 = 0;
    cJSON_bool added_n2 = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;
    double pi_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    pi_item = cJSON_AddNumberToObject(root, "pi", 3.14);
    flag_item = cJSON_CreateFalse();
    added_flag = cJSON_AddItemToObject(root, "flag", flag_item);
    added_array = cJSON_AddItemToObject(root, "list", arr);
    added_n1 = cJSON_AddItemToArray(arr, n1);
    added_n2 = cJSON_AddItemToArray(arr, n2);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_flag);
    retrieved_pi = cJSON_GetObjectItem(root, "pi");
    pi_value = cJSON_GetNumberValue(retrieved_pi);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_flag + (int)added_array + (int)added_n1 + (int)added_n2 + (int)is_false + (int)(retrieved_pi != (cJSON *)0) + (int)(pi_value == 3.14);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_flag;
    (void)retrieved_pi;
    (void)pi_item;
    (void)flag_item;
    (void)n1;
    (void)n2;
    // API sequence test completed successfully
    return 66;
}