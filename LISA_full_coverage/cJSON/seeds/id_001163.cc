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
//<ID> 1163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"a\": 12.25, \"arr\": [1,2], \"obj\": {\"nested\":3}}";
    size_t len1 = strlen(json1);
    cJSON *parsed1 = cJSON_ParseWithLength(json1, len1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *item_a = cJSON_GetObjectItem(parsed1, "a");
    double val_a = cJSON_GetNumberValue(item_a);
    cJSON *num_copy = cJSON_CreateNumber(val_a * 2.0);
    cJSON_AddItemToObject(root, "double_a", num_copy);
    cJSON *raw_item = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_flag);

    // step 3: Operate
    const char *json2 = "{\"x\": 7} TRASH";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json2, &parse_end, 0);
    cJSON *item_x = cJSON_GetObjectItem(parsed2, "x");
    double val_x = cJSON_GetNumberValue(item_x);
    cJSON_AddNumberToObject(root, "x_from_parsed", val_x + 1.5);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}