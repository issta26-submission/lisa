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
//<ID> 655
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums1[3] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateDoubleArray(nums1, 3);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddStringToObject(root, "creator", "tester");

    // step 2: Configure / Serialize
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buflen = 256;
    char *buffer = (char *)cJSON_malloc(buflen);
    memset(buffer, 0, buflen);
    sprintf(buffer, "%s", snapshot);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int original_count = cJSON_GetArraySize(parsed_values);
    double nums2[2] = {9.9, 8.8};
    cJSON *new_arr = cJSON_CreateDoubleArray(nums2, 2);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "values", new_arr);
    int replaced_count = cJSON_GetArraySize(cJSON_GetObjectItem(parsed, "values"));
    (void)original_count;
    (void)replaced_count;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}