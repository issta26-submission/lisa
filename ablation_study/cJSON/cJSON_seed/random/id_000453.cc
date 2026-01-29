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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *str = cJSON_CreateString("value");

    // step 2: Configure
    double setval = cJSON_SetNumberHelper(num, 42.5);
    cJSON *num_copy = cJSON_Duplicate(num, 1);
    cJSON_AddItemToArray(arr, num_copy);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "num", num);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_num = cJSON_GetNumberValue(first);
    cJSON_bool has_num = cJSON_HasObjectItem(root, "num");

    // step 4: Validate
    int validation = 0;
    validation ^= (int)first_num;
    validation ^= (int)setval;
    validation ^= (int)has_num;
    (void)validation;

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}