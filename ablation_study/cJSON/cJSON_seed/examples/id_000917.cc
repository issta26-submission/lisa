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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("alpha");
    cJSON *t = cJSON_CreateTrue();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *obj = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToArray(root, str1);
    cJSON_AddItemToArray(root, t);
    cJSON_AddItemToArray(root, num);
    cJSON_bool eq1 = cJSON_Compare(str1, str2, 1);
    cJSON_AddBoolToObject(obj, "initial_equal", eq1);

    // step 3: Operate & Validate
    cJSON *num_from_array = cJSON_GetArrayItem(root, 2);
    double updated_value = cJSON_SetNumberHelper(num_from_array, 123.456);
    cJSON_bool eq2 = cJSON_Compare(str1, cJSON_GetArrayItem(root, 0), 1);
    cJSON_AddBoolToObject(obj, "post_add_equal", eq2);
    (void)updated_value;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(str2);
    cJSON_Delete(obj);
    return 66;
    // API sequence test completed successfully
}