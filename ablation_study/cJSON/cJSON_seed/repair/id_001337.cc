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
//<ID> 1337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *numbers = cJSON_AddArrayToObject(child, "numbers");
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);
    int size_before = cJSON_GetArraySize(numbers);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(numbers, 1);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(numbers, second_item, replacement);
    int size_after = cJSON_GetArraySize(numbers);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 2));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + ((int)v0 % 10));
    ((char *)scratch)[2] = (char)('0' + ((int)v1 % 10));
    ((char *)scratch)[3] = (char)('0' + ((int)v2 % 10));
    ((char *)scratch)[4] = (char)('0' + (size_before % 10));
    ((char *)scratch)[5] = (char)('0' + (size_after % 10));
    ((char *)scratch)[6] = (char)('0' + (replaced ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}