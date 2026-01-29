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
//<ID> 1190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON *name = cJSON_AddStringToObject(root, "name", "tester");
    cJSON *opt = cJSON_AddNullToObject(root, "optional");
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 42.5);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 2: Configure / Parse
    const char embed_json[] = "{\"embedded\": {\"x\":1, \"y\":2}, \"arr\": [\"a\",\"b\"]}";
    size_t embed_len = sizeof(embed_json) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(embed_json, embed_len, &parse_end, 1);

    // step 3: Operate and Validate
    cJSON *embedded = cJSON_GetObjectItem(parsed, "embedded");
    cJSON *x_item = cJSON_GetObjectItem(embedded, "x");
    cJSON *y_item = cJSON_GetObjectItem(embedded, "y");
    double x_val = cJSON_GetNumberValue(x_item);
    double y_val = cJSON_GetNumberValue(y_item);
    cJSON_AddNumberToObject(cfg, "sum", x_val + y_val);
    cJSON *detached = cJSON_DetachItemViaPointer(root, num_item);
    cJSON_AddItemToObject(cfg, "detached_value", detached);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}