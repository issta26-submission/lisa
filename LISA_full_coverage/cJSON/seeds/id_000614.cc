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
//<ID> 614
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.0, 2.0, 3.0};
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "scale", 2.5);
    cJSON_AddStringToObject(meta, "info", "detachable");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "temp", "will_remove");

    // step 2: Configure
    cJSON *numbers_ref = cJSON_GetObjectItem(root, "numbers");
    cJSON *middle_item = cJSON_GetArrayItem(numbers_ref, 1);
    double middle_val = cJSON_GetNumberValue(middle_item);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta_ref, "scale");
    double scale_val = cJSON_GetNumberValue(scale_item);
    double scaled = middle_val * scale_val;

    // step 3: Operate
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, detached_meta);
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON_DeleteItemFromObject(root, "temp");
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON *refs_ref = cJSON_GetObjectItem(root, "refs");
    cJSON *first_ref = cJSON_GetArrayItem(refs_ref, 0);
    cJSON *first_scale = cJSON_GetObjectItem(first_ref, "scale");
    double first_scale_val = cJSON_GetNumberValue(first_scale);
    (void)first_scale_val;
    (void)scaled;
    (void)middle_val;
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}