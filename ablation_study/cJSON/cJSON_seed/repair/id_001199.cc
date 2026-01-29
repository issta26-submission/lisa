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
//<ID> 1199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_AddStringToObject(meta, "label", "initial");
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    const char sample_json[] = "{\"parsed_key\":\"parsed_val\"}";
    size_t sample_len = sizeof(sample_json) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(sample_json, sample_len, &parse_end, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double prev_num = cJSON_SetNumberHelper(count_item, 5.5);
    (void)prev_num;

    // step 3: Operate and Validate
    cJSON *maybe_ptr = cJSON_GetObjectItem(root, "maybe");
    cJSON *detached = cJSON_DetachItemViaPointer(root, maybe_ptr);
    cJSON_AddItemToObject(parsed, "reinserted", detached);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    const char *lbl = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "label"));
    double count_val = cJSON_GetNumberValue(count_item);
    double final_val = count_val + (double)((unsigned char)out[0]);
    cJSON_AddNumberToObject(root, "final", final_val);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}