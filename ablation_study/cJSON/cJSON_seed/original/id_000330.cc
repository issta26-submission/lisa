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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int nums1[] = {1, 2, 3, 4};
    cJSON *ints = cJSON_CreateIntArray(nums1, 4);
    cJSON_AddItemToObjectCS(root, "ints", ints);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *replacement_null = cJSON_CreateNull();
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, ints, replacement_null);
    int nums2[] = {10, 20};
    cJSON *ints_v2 = cJSON_CreateIntArray(nums2, 2);
    cJSON_AddItemToObjectCS(meta, "ints_v2", ints_v2);
    cJSON_AddItemToObjectCS(meta, "replaced", cJSON_CreateBool(replaced));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_bool pre_alloc_ok = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    cJSON_AddItemToObjectCS(meta, "printed_prealloc_ok", cJSON_CreateBool(pre_alloc_ok));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}