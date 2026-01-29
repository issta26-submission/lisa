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
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *flag_key = "enabled";
    double nums[3] = {10.0, 20.0, 30.0};
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON_bool is_false = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(nums[0]);
    n1 = cJSON_CreateNumber(nums[1]);
    n2 = cJSON_CreateNumber(nums[2]);
    flag_item = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach items to root object
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, arr_key, arr);
    cJSON_AddItemToObject(root, flag_key, flag_item);

    // step 4: Operate - retrieve array and its first element, check boolean type
    retrieved_arr = cJSON_GetObjectItem(root, arr_key);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, flag_key));

    // step 5: Validate - print and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(first_elem != (cJSON *)0) + (int)is_false + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}