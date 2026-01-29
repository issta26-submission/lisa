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
//<ID> 856
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", num_item);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    const char *json_text = "{\"external\":{\"value\":3.14},\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *external_obj = cJSON_GetObjectItem(parsed, "external");
    cJSON *external_ref = cJSON_CreateObjectReference(external_obj);
    cJSON_AddItemToObject(root, "external_ref", external_ref);

    // step 3: Operate and Validate
    cJSON *child_obj = cJSON_GetObjectItem(root, "child");
    cJSON *answer_item = cJSON_GetObjectItem(child_obj, "answer");
    double answer_val = cJSON_GetNumberValue(answer_item);
    cJSON *ext_ref_obj = cJSON_GetObjectItem(root, "external_ref");
    cJSON *ext_value_item = cJSON_GetObjectItem(ext_ref_obj, "value");
    double ext_val = cJSON_GetNumberValue(ext_value_item);
    double sum = answer_val + ext_val;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}