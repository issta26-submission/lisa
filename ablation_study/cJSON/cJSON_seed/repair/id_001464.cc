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
//<ID> 1464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateTrue());

    // step 2: Configure
    const char *json_text = "{\"parsedKey\":{\"nested\":[7,8,9]}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsedKey = cJSON_GetObjectItem(parsed, "parsedKey");
    cJSON *nested = cJSON_GetObjectItem(parsedKey, "nested");
    cJSON *dup_nested = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "dup_nested", dup_nested);

    // step 3: Operate and Validate
    int nested_size = cJSON_GetArraySize(nested);
    int items_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "orig_count", (double)nested_size);
    cJSON_AddNumberToObject(root, "items_count", (double)items_count);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = (char)('0' + (nested_size % 10));
    scratch[2] = (char)('0' + (items_count % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}