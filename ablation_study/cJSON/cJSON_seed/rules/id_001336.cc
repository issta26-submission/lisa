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
//<ID> 1336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"metrics\":{\"value\":42.5,\"label\":\"initial\"}}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *metrics = NULL;
    cJSON *value_item = NULL;
    cJSON *label_item = NULL;
    cJSON *computed_item = NULL;
    cJSON *createdNum = NULL;
    cJSON *createdStr = NULL;
    cJSON *dup_metrics = NULL;
    char *printed = NULL;
    double orig_value = 0.0;
    double new_value = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    metrics = cJSON_GetObjectItem(root, "metrics");
    value_item = cJSON_GetObjectItem(metrics, "value");
    label_item = cJSON_GetObjectItem(metrics, "label");
    orig_value = cJSON_GetNumberValue(value_item);

    // step 3: Operate / Validate
    createdNum = cJSON_CreateNumber(orig_value + 7.5);
    createdStr = cJSON_CreateString("computed_label");
    cJSON_AddItemToObject(root, "computed", createdNum);
    cJSON_AddItemToObject(root, "note", createdStr);
    computed_item = cJSON_GetObjectItem(root, "computed");
    new_value = cJSON_GetNumberValue(computed_item);
    dup_metrics = cJSON_Duplicate(metrics, 1);
    equal = cJSON_Compare(metrics, dup_metrics, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)parse_end;
    (void)label_item;
    (void)orig_value;
    (void)new_value;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_metrics);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}