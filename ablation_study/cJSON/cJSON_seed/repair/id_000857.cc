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
//<ID> 857
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"tester\",\"metrics\":{\"cpu\":0.5}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *new_root = cJSON_CreateObject();
    cJSON *number_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(new_root, "answer", number_item);
    cJSON *metrics = cJSON_GetObjectItem(parsed, "metrics");
    cJSON *metrics_ref = cJSON_CreateObjectReference(metrics);
    cJSON_AddItemToObject(new_root, "metrics_ref", metrics_ref);

    // step 3: Operate and Validate
    cJSON *ans_item = cJSON_GetObjectItem(new_root, "answer");
    double ans_val = cJSON_GetNumberValue(ans_item);
    cJSON_AddNumberToObject(new_root, "answer_copy", ans_val);
    cJSON *metrics_ref_item = cJSON_GetObjectItem(new_root, "metrics_ref");
    cJSON *cpu_item = cJSON_GetObjectItem(metrics_ref_item, "cpu");
    double cpu_val = cJSON_GetNumberValue(cpu_item);
    cJSON_AddNumberToObject(new_root, "cpu_copy", cpu_val);
    char *snapshot_root = cJSON_PrintUnformatted(new_root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(new_root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}