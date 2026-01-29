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
//<ID> 1191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_AddStringToObject(root, "name", "initial");
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(5.0);
    cJSON_AddItemToObject(config, "threshold", threshold);
    cJSON_SetNumberHelper(threshold, 10.5);

    const char json[] = "{\"external\":\"yes\",\"num\":7}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, maybe);
    cJSON_AddStringToObject(parsed, "origin", "imported");
    cJSON *parsed_num_item = cJSON_GetObjectItem(parsed, "num");
    double parsed_num = cJSON_GetNumberValue(parsed_num_item);
    double thresh = cJSON_GetNumberValue(threshold);
    double final_val = thresh + parsed_num;
    cJSON_AddNumberToObject(root, "final", final_val);

    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}