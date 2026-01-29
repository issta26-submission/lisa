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
//<ID> 914
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
    cJSON *s1 = cJSON_CreateString("TestValue");
    cJSON *s2 = cJSON_CreateString("TestValue");
    cJSON *t = cJSON_CreateTrue();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 7.5);

    // step 2: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "my_array", arr);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool are_equal = cJSON_Compare(s1, second, 1);
    cJSON_AddBoolToObject(root, "items_equal", are_equal);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}