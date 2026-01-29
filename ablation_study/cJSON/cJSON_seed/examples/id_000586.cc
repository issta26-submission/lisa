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
//<ID> 586
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
    cJSON *values = cJSON_CreateArray();

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON_AddItemToObjectCS(root, "values", values);
    cJSON_AddStringToObject(root, "label", "example");
    cJSON_AddNumberToObject(root, "score", 3.14);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int size_original = cJSON_GetArraySize(parsed_values);
    cJSON *dup_values = cJSON_Duplicate(parsed_values, 1);
    int size_dup = cJSON_GetArraySize(dup_values);
    cJSON *label_item = cJSON_GetObjectItem(parsed, "label");
    const char *label_str = cJSON_GetStringValue(label_item);
    cJSON *score_item = cJSON_GetObjectItem(parsed, "score");
    double score = cJSON_GetNumberValue(score_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;orig_size=%d;dup_size=%d;label=%s;score=%.2f",
            version ? version : "null",
            size_original,
            size_dup,
            label_str ? label_str : "null",
            score);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(dup_values);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}