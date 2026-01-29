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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *num_node = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "score", num_node);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddItemToObject(root, "missing", cJSON_CreateNull());
    cJSON_AddStringToObject(root, "name", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed, "name");
    char *name_val = (char *)cJSON_GetStringValue(parsed_name_item);
    cJSON *parsed_score_item = cJSON_GetObjectItem(parsed, "score");
    double score_val = cJSON_GetNumberValue(parsed_score_item);
    cJSON *detached_score = cJSON_DetachItemFromObject(parsed, "score");
    double detached_score_val = cJSON_GetNumberValue(detached_score);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;name=%s;score=%.2f;detached=%.2f",
            version ? version : "null",
            name_val ? name_val : "null",
            score_val,
            detached_score_val);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_score);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}