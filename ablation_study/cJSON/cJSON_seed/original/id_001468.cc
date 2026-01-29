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
//<ID> 1468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"items\":[10,20],\"meta\":{\"name\":\"orig\"},\"extra\":\"value\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");

    // step 2: Configure
    cJSON *ref_number = cJSON_CreateNumber(99.0);
    cJSON_AddItemReferenceToArray(items, ref_number);
    cJSON *new_meta = cJSON_CreateString("replaced_meta");
    cJSON_ReplaceItemInObject(root, "meta", new_meta);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *replacement_for_first = cJSON_CreateNumber(777.0);
    cJSON_ReplaceItemViaPointer(items, first_item, replacement_for_first);

    // step 3: Operate & Validate
    cJSON_bool meta_is_invalid = cJSON_IsInvalid(new_meta);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    (void)meta_is_invalid;
    (void)second_val;
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}