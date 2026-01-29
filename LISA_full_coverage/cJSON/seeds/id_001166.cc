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
//<ID> 1166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"value\": 42.5, \"raw\": \"placeholder\"}";
    size_t json1_len = sizeof(json1) - 1;
    cJSON *root1 = cJSON_ParseWithLength(json1, json1_len);
    const char json2[] = "{\"value\": 100.0, \"raw\": \"initial_raw\"}";
    const char *parse_end2 = NULL;
    cJSON *root2 = cJSON_ParseWithOpts(json2, &parse_end2, 1);

    // step 2: Configure
    cJSON *num_item1 = cJSON_GetObjectItem(root1, "value");
    double extracted_value = cJSON_GetNumberValue(num_item1);
    cJSON *raw_item2 = cJSON_GetObjectItem(root2, "raw");
    cJSON_bool raw_before = cJSON_IsRaw(raw_item2);
    cJSON *raw_fragment = cJSON_CreateRaw("<<RAW_FRAGMENT>>");
    cJSON_ReplaceItemViaPointer(root2, raw_item2, raw_fragment);
    cJSON_bool raw_after = cJSON_IsRaw(raw_fragment);

    // step 3: Operate
    cJSON *merged = cJSON_CreateObject();
    cJSON_AddNumberToObject(merged, "extracted", extracted_value);
    cJSON *dup_root2 = cJSON_Duplicate(root2, 1);
    cJSON_AddItemToObject(merged, "source_copy", dup_root2);
    cJSON *num_added = cJSON_GetObjectItem(merged, "extracted");
    double verified_value = cJSON_GetNumberValue(num_added);
    (void)raw_before;
    (void)raw_after;
    (void)verified_value;

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(merged);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(merged, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root1);
    cJSON_Delete(root2);
    cJSON_Delete(merged);

    // API sequence test completed successfully
    return 66;
}