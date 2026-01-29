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
//<ID> 806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("example");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(7.5);

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddRawToObject(root, "payload", "{\"embedded\":42}");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(out);
    cJSON_free(out);
    cJSON *flag = cJSON_GetObjectItemCaseSensitive(parsed, "flag");
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    cJSON *payload = cJSON_GetObjectItemCaseSensitive(parsed, "payload");
    cJSON *embedded = cJSON_GetObjectItemCaseSensitive(payload, "embedded");
    double embedded_val = cJSON_GetNumberValue(embedded);

    // step 4: Validate and Cleanup
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddBoolToObject(summary, "flag_is_true", flag_is_true);
    cJSON_AddNumberToObject(summary, "embedded_val", embedded_val);
    char *summary_out = cJSON_PrintUnformatted(summary);
    cJSON_free(summary_out);
    cJSON_Delete(summary);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}