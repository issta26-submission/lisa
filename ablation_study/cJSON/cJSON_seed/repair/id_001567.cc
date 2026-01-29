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
//<ID> 1567
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
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON_InsertItemInArray(array, 0, item0);
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON_InsertItemInArray(array, 1, item1);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(array);
    cJSON *item2 = cJSON_CreateString("gamma");
    cJSON_InsertItemInArray(array, size_before, item2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON *detached_array = cJSON_DetachItemFromObject(root, "items");

    // step 3: Operate and Validate
    int array_size = cJSON_GetArraySize(detached_array);
    cJSON *first_elem = cJSON_GetArrayItem(detached_array, 0);
    const char *first_str = cJSON_GetStringValue(first_elem);
    char *printed = cJSON_PrintBuffered(detached_array, 64, 1);
    size_t printed_len = printed ? strlen(printed) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(printed_len + 32);
    memset(scratch, 0, printed_len + 32);
    scratch[0] = first_str[0];
    scratch[1] = (char)('0' + (int)array_size);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(detached_array);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}