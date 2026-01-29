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
//<ID> 1105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_AddObjectToObject(root, "metadata");
    cJSON *flag = cJSON_AddFalseToObject(metadata, "active");
    int nums[3] = {7, 8, 9};
    cJSON *ints = cJSON_CreateIntArray(nums, 3);
    cJSON_AddItemToObject(root, "ints", ints);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *ints_ref = cJSON_CreateArrayReference(ints);
    cJSON_AddItemToObject(metadata, "ints_ref", ints_ref);
    cJSON_AddItemToArray(list, cJSON_CreateString("entry1"));
    cJSON_AddItemToArray(list, cJSON_CreateString("entry2"));

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    int size_ints = cJSON_GetArraySize(ints);
    int is_bool = (int)cJSON_IsBool(flag);
    int is_false = (int)cJSON_IsFalse(flag);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (size_ints & 0xF));
    ((char *)scratch)[2] = (char)('0' + (is_bool & 1));
    ((char *)scratch)[3] = (char)('0' + (is_false & 1));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(scratch);
    // API sequence test completed successfully
    return 66;
}