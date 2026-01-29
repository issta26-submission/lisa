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
//<ID> 1160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"val\":123.5, \"raw\":\"<raw>data</raw>\", \"extras\": [1,2,3]}garbage";
    const char *parse_end1 = NULL;
    cJSON *parsed1 = cJSON_ParseWithOpts(json1, &parse_end1, 0);
    cJSON *val_item = cJSON_GetObjectItem(parsed1, "val");
    double val = cJSON_GetNumberValue(val_item);
    cJSON *raw_item = cJSON_GetObjectItem(parsed1, "raw");
    cJSON_bool is_raw_before = cJSON_IsRaw(raw_item);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "parsed_value", val);
    cJSON_AddRawToObject(root, "raw_blob", "<raw>xyz</raw>");
    cJSON *raw_blob = cJSON_GetObjectItem(root, "raw_blob");
    cJSON_bool is_raw_after = cJSON_IsRaw(raw_blob);

    // step 3: Operate
    const char *json2 = "{\"n\": 7.2}";
    size_t json2_len = sizeof("{\"n\": 7.2}") - 1;
    cJSON *parsed2 = cJSON_ParseWithLength(json2, json2_len);
    cJSON *n_item = cJSON_GetObjectItem(parsed2, "n");
    double nval = cJSON_GetNumberValue(n_item);
    cJSON_AddNumberToObject(root, "n_value", nval);
    char *out = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(out);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    (void)is_raw_before;
    (void)is_raw_after;
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}