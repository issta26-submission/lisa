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
//<ID> 1569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *item1 = cJSON_CreateString("alpha");
    cJSON_InsertItemInArray(array, 0, item1);
    cJSON *item2 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(array, 1, item2);
    cJSON *map = cJSON_CreateObject();
    cJSON *label = cJSON_CreateString("detached_label");
    cJSON_AddItemToObject(map, "label", label);
    cJSON_AddItemToObject(root, "map", map);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(array);
    cJSON *item3 = cJSON_CreateString("gamma");
    cJSON_InsertItemInArray(array, size_before, item3);
    int size_after = cJSON_GetArraySize(array);

    // step 3: Operate and Validate
    cJSON *detached_map = cJSON_DetachItemFromObject(root, "map");
    cJSON *detached_label = cJSON_GetObjectItem(detached_map, "label");
    char *label_str = cJSON_GetStringValue(detached_label);
    char *out = cJSON_PrintBuffered(root, 256, 1);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = label_str ? label_str[0] : ' ';
    scratch[2] = (char)('0' + (int)size_after);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(detached_map);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}