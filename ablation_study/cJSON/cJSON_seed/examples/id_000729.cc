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
//<ID> 729
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON *false_item = cJSON_CreateFalse();
    (void)version;

    // step 2: Configure
    cJSON_AddItemToArray(array, dbl_arr);
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, false_item);
    cJSON_AddItemToObject(root, "payload", array);
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("example_title"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(parsed, "payload");
    cJSON *first_elem = cJSON_GetArrayItem(detached, 0);
    cJSON *inner_second = cJSON_GetArrayItem(first_elem, 1);
    double inner_val = cJSON_GetNumberValue(inner_second);
    cJSON *second_str_item = cJSON_GetArrayItem(detached, 1);
    const char *second_str = cJSON_GetStringValue(second_str_item);
    (void)inner_val;
    (void)second_str;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}