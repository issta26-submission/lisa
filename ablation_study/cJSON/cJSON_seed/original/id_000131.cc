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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *fruits[] = { "apple", "Banana", "cherry" };
    cJSON *strArr = cJSON_CreateStringArray(fruits, 3);
    float nums[] = { 1.1f, 2.2f, 3.3f };
    cJSON *fltArr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "fruits", strArr);
    cJSON_AddItemToObject(root, "values", fltArr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "source", "synthesized");
    cJSON_AddItemToObject(root, "meta", meta);
    const char *fruits_copy[] = { "apple", "Banana", "cherry" };
    cJSON *strArr2 = cJSON_CreateStringArray(fruits_copy, 3);
    cJSON_AddItemToObject(root, "fruits_copy", strArr2);
    cJSON_bool same_arrays = cJSON_Compare(strArr, strArr2, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    int count = cJSON_GetArraySize(strArr);
    int summary = (printed ? (int)printed[0] : 0) + (int)count + (int)same_arrays + (int)root->type;
    (void)summary;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}