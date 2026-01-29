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
//<ID> 1469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[10,20,30],\"nested\":{\"k\":\"v\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    int count = cJSON_GetArraySize(arr);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "original_size", (double)count);
    cJSON_AddItemToObject(dup, "summary", summary);

    // step 3: Operate and Validate
    cJSON *got_summary = cJSON_GetObjectItem(dup, "summary");
    cJSON *orig_size_item = cJSON_GetObjectItem(got_summary, "original_size");
    double orig_val = cJSON_GetNumberValue(orig_size_item);
    char *out = cJSON_PrintUnformatted(dup);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + ((int)orig_val % 10));
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('S');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}