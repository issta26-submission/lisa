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
//<ID> 980
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
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON *str1 = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, str1);
    cJSON *str2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, str2);
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(arr, num);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    cJSON *newstr = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, newstr);
    cJSON_SetNumberHelper(num, 3.14);
    double current = cJSON_GetNumberValue(num);
    cJSON *num_copy = cJSON_CreateNumber(current);
    cJSON_AddItemToObject(root, "num_copy", num_copy);
    char *s = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    cJSON *copied = cJSON_CreateString(s);
    cJSON_AddItemToObject(root, "copied", copied);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}