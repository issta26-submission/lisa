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
//<ID> 859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"user\":\"bob\",\"metrics\":{\"score\":99.5,\"level\":2}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *number_node = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", number_node);
    cJSON *parsed_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObject(root, "parsed_ref", parsed_ref);
    cJSON_AddItemToObject(root, "imported", parsed);
    parsed = NULL;

    // step 3: Operate and Validate
    cJSON *imported = cJSON_GetObjectItem(root, "imported");
    cJSON *metrics = cJSON_GetObjectItem(imported, "metrics");
    cJSON *score_item = cJSON_GetObjectItem(metrics, "score");
    double score_val = cJSON_GetNumberValue(score_item);
    cJSON *score_copy = cJSON_CreateNumber(score_val);
    cJSON_AddItemToObject(root, "score_copy", score_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}