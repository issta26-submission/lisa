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
//<ID> 658
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
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *status = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(root, "status", status);
    cJSON_AddNumberToObject(root, "initial_count", 7.0);

    // step 2: Configure
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "payload", payload);
    cJSON_AddNumberToObject(payload, "value", 3.14);
    cJSON_AddNumberToObject(payload, "multiplier", 2.0);
    cJSON_bool has_status = cJSON_HasObjectItem(root, "status");
    cJSON_AddNumberToObject(root, "status_present", (double)has_status);

    // step 3: Operate and Validate
    cJSON *val_item = cJSON_GetObjectItem(payload, "value");
    double val = cJSON_GetNumberValue(val_item);
    cJSON *mul_item = cJSON_GetObjectItem(payload, "multiplier");
    double mul = cJSON_GetNumberValue(mul_item);
    double calc = val * mul;
    cJSON_AddNumberToObject(root, "calc_result", calc);
    cJSON *version = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObjectCS(meta, "version", version);

    // step 4: Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}