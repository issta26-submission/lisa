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
//<ID> 860
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums1[] = {1.1f, 2.2f, 3.3f};
    cJSON *array1 = cJSON_CreateFloatArray(nums1, 3);
    cJSON_AddItemToObject(root, "floats", array1);
    cJSON *flag_item = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    char *serialized = cJSON_PrintUnformatted(root);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(serialized, &parse_end, 1);
    float nums2[] = {4.4f, 5.5f};
    cJSON *array2 = cJSON_CreateFloatArray(nums2, 2);
    cJSON_ReplaceItemInObject(root, "floats", array2);

    // step 3: Operate
    cJSON *floats_obj = cJSON_GetObjectItem(root, "floats");
    int count = cJSON_GetArraySize(floats_obj);
    cJSON *first_elem = cJSON_GetArrayItem(floats_obj, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    (void)count;
    (void)first_value;
    (void)flag_item;
    (void)parse_end;

    // step 4: Validate & Cleanup
    cJSON_free(serialized);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}