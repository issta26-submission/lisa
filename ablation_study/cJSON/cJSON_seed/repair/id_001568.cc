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
//<ID> 1568
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
    cJSON_AddItemToArray(array, item0);
    cJSON *item1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(array, item1);
    cJSON *item_insert = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(array, 1, item_insert);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *first_elem = cJSON_GetArrayItem(array, 0);
    char *first_text = cJSON_GetStringValue(first_elem);
    cJSON *first_copy = cJSON_CreateString(first_text);
    cJSON_AddItemToObject(meta, "first_copy", first_copy);

    // step 3: Operate and Validate
    int prebuf = size_before * 16 + 32;
    char *printed = cJSON_PrintBuffered(root, prebuf, 0);
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON *detached_first = cJSON_GetObjectItem(detached_meta, "first_copy");
    char *detached_text = cJSON_GetStringValue(detached_first);
    int scratch_size = prebuf + 64;
    char *scratch = (char *)cJSON_malloc((size_t)scratch_size);
    memset(scratch, 0, (size_t)scratch_size);
    scratch[0] = detached_text ? detached_text[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}