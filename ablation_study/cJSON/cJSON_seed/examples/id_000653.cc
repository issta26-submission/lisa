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
//<ID> 653
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
    double nums1[3] = {1.1, 2.2, 3.3};
    cJSON *array1 = cJSON_CreateDoubleArray(nums1, 3);
    cJSON_AddItemToObject(root, "values", array1);
    cJSON_AddStringToObject(root, "creator", "fuzzer");

    // step 2: Configure
    double nums2[2] = {4.4, 5.5};
    cJSON *array2 = cJSON_CreateDoubleArray(nums2, 2);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values", array2);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(512);
    memset(buffer, 0, 512);
    sprintf((char *)buffer, "%s", snapshot);
    cJSON_Minify((char *)buffer);
    cJSON *parsed = cJSON_Parse((const char *)buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int count = cJSON_GetArraySize(parsed_values);
    (void)version;
    (void)count;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}