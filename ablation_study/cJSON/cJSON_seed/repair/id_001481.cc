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
//<ID> 1481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[10,20,30],\"meta\":{\"val\":42.5}}";
    size_t json_len = (size_t)strlen(json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double removed_value = cJSON_GetNumberValue(second_item);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *meta_val_item = cJSON_GetObjectItemCaseSensitive(meta, "val");
    double meta_val = cJSON_GetNumberValue(meta_val_item);

    // step 2: Configure
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "removed_value", removed_value);
    cJSON_AddNumberToObject(summary, "meta_val", meta_val);
    cJSON_AddItemToObject(root, "summary", summary);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 1);
    int new_count = cJSON_GetArraySize(arr);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + ((int)removed_value % 10));
    scratch[1] = (char)('0' + (new_count % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + ((int)meta_val % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}