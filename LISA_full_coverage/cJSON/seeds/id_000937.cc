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
//<ID> 937
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"label\":\"orig\",\"values\":[1.0,2.0]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    float nums[3] = {3.14f, 2.71f, 1.62f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObjectCS(root, "extra_floats", float_arr);
    cJSON *arr_ref = cJSON_CreateArrayReference(float_arr);
    cJSON_AddItemToObjectCS(root, "extra_floats_ref", arr_ref);
    cJSON *note = cJSON_CreateString("added_note");
    cJSON_AddItemToObjectCS(root, "note", note);

    // step 3: Operate
    cJSON *label_item = cJSON_GetObjectItemCaseSensitive(root, "label");
    const char *label_str = cJSON_GetStringValue(label_item);
    cJSON *label_copy = cJSON_CreateString(label_str ? label_str : "");
    cJSON_AddItemToObjectCS(root, "label_copy", label_copy);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    int buflen = 256;
    char *buf = (char *)cJSON_malloc((size_t)buflen);
    memset(buf, 0, buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buf, buflen, 1);
    (void)ok;
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}