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
//<ID> 1578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *json_text = "{\"meta\":{\"count\":2},\"items\":[1,2]}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *fixed_number = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "ok", true_item);
    cJSON_AddItemToObject(root, "answer", fixed_number);

    // step 3: Operate and Validate
    cJSON *meta_count = cJSON_GetObjectItem(meta, "count");
    double count_val = cJSON_GetNumberValue(meta_count);
    cJSON *count_copy = cJSON_CreateNumber(count_val);
    cJSON_AddItemToObject(root, "count_copy", count_copy);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)meta_is_obj);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (int)count_val);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}