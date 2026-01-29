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
//<ID> 1521
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *num_prop = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_elem0 = 0;
    cJSON_bool added_elem1 = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    flag = cJSON_CreateFalse();

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "items", arr);
    num_prop = cJSON_AddNumberToObject(root, "initial_count", 0.0);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 4: Operate
    elem0 = cJSON_CreateNumber(10.0);
    elem1 = cJSON_CreateNumber(20.0);
    added_elem0 = cJSON_AddItemToArray(arr, elem0);
    added_elem1 = cJSON_AddItemToArray(arr, elem1);
    printed = cJSON_PrintUnformatted(root);
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_arr + (int)added_elem0 + (int)added_elem1 + (int)(retrieved_arr != (cJSON *)0) + (int)is_false + (int)(num_prop != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_arr;
    (void)retrieved_flag;
    (void)num_prop;
    (void)elem0;
    (void)elem1;
    // API sequence test completed successfully
    return 66;
}