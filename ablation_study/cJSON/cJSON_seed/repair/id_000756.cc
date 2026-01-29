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
//<ID> 756
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "optional", null_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(child, "raw", raw_item);
    cJSON *greet = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greet);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(metrics, "answer", answer);

    // step 3: Operate and Validate
    cJSON *got_greet = cJSON_GetObjectItem(root, "greeting");
    const char *greet_val = cJSON_GetStringValue(got_greet);
    (void)greet_val;
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_raw = cJSON_GetObjectItem(got_child, "raw");
    (void)got_raw;
    cJSON *got_optional = cJSON_GetObjectItem(got_child, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(got_optional);
    (void)optional_is_null;
    cJSON *metrics_obj = cJSON_GetObjectItem(root, "metrics");
    cJSON *answer_obj = cJSON_GetObjectItem(metrics_obj, "answer");
    double answer_val = cJSON_GetNumberValue(answer_obj);
    (void)answer_val;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}