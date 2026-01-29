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
//<ID> 1534
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
    cJSON *str_item1 = (cJSON *)0;
    cJSON *str_item2 = (cJSON *)0;
    cJSON *single_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_single = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *single_value = (char *)0;
    char *first_value = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item1 = cJSON_CreateString("alpha");
    str_item2 = cJSON_CreateString("beta");
    single_item = cJSON_CreateString("singleton");

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, str_item1);
    added2 = cJSON_AddItemToArray(arr, str_item2);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddItemToObject(root, "single", single_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    retrieved_single = cJSON_GetObjectItem(root, "single");
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    single_value = (char *)cJSON_GetStringValue(retrieved_single);
    first_value = (char *)cJSON_GetStringValue(first_elem);

    // step 5: Validate
    validation_score = (int)added1 + (int)added2 + (int)(retrieved_arr != (cJSON *)0) + (int)(retrieved_single != (cJSON *)0) + (int)(single_value != (char *)0) + (int)(first_value != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_value;
    (void)single_value;
    (void)retrieved_single;
    (void)retrieved_arr;
    (void)first_elem;
    (void)str_item2;
    (void)str_item1;
    (void)single_item;
    // API sequence test completed successfully
    return 66;
}