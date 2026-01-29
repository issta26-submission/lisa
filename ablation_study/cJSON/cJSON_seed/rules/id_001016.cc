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
//<ID> 1016
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *measurements = NULL;
    cJSON *replacement_measurements = NULL;
    cJSON *meta = NULL;
    cJSON *active = NULL;
    cJSON *tmp = NULL;
    char *printed = NULL;
    const double nums1[] = {1.1, 2.2, 3.3};
    const double nums2[] = {4.4, 5.5};
    cJSON_bool active_state = 0;
    int measurements_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    measurements = cJSON_CreateDoubleArray(nums1, 3);
    cJSON_AddItemToObject(root, "measurements", measurements);
    active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);
    meta = cJSON_CreateRaw("{\"version\":1}");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    replacement_measurements = cJSON_CreateDoubleArray(nums2, 2);
    tmp = cJSON_GetObjectItem(root, "measurements");
    cJSON_ReplaceItemViaPointer(root, tmp, replacement_measurements);
    tmp = cJSON_GetObjectItem(root, "active");
    active_state = cJSON_IsTrue(tmp);
    measurements_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "measurements"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}