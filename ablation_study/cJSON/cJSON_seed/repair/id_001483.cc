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
//<ID> 1483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[10,20,30],\"meta\":{\"val\":42.5}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)strlen(json), &parse_end, 1);
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *val_item = cJSON_GetObjectItemCaseSensitive(meta, "val");
    double val_before = cJSON_GetNumberValue(val_item);

    // step 2: Configure
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddNumberToObject(extra, "added", 7.5);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *item_after = cJSON_GetArrayItem(arr, 1);
    double num_after = cJSON_GetNumberValue(item_after);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = (char)('0' + ((int)val_before % 10));
    scratch[1] = (char)('0' + ((int)num_after % 10));
    scratch[2] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}