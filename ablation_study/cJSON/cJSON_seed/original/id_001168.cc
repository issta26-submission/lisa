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
//<ID> 1168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"value\": 123.45, \"raw_field\": \"rdata\"}";
    size_t len1 = sizeof(json1) - 1;
    cJSON *parsed1 = cJSON_ParseWithLength(json1, len1);
    const char json2[] = "{\"opt\": 99.5}TRAIL";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json2, &parse_end, 0);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *value_item = cJSON_GetObjectItem(parsed1, "value");
    double val = cJSON_GetNumberValue(value_item);
    cJSON_AddNumberToObject(root, "copied_value", val);
    cJSON *opt_item = cJSON_GetObjectItem(parsed2, "opt");
    double optval = cJSON_GetNumberValue(opt_item);
    cJSON_AddNumberToObject(root, "opt_value", optval);
    cJSON *raw_item = cJSON_CreateRaw("embedded_raw_payload");
    cJSON_AddItemToObject(parsed1, "embedded_raw", raw_item);

    // step 3: Operate & Validate
    cJSON *embedded = cJSON_GetObjectItem(parsed1, "embedded_raw");
    cJSON_bool embedded_is_raw = cJSON_IsRaw(embedded);
    cJSON_AddBoolToObject(root, "embedded_is_raw", embedded_is_raw);
    cJSON *dup = cJSON_Duplicate(parsed1, 1);
    cJSON_AddItemToObject(root, "parsed_dup", dup);
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Cleanup
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}