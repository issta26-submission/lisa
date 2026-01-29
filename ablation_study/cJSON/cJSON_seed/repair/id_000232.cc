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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", false_item);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON *first = cJSON_CreateString("first");
    cJSON_InsertItemInArray(arr, 0, first);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(nested, "desc", cJSON_CreateString("nested object"));
    cJSON_AddItemToObject(root, "nested", nested);
    double ans = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    cJSON *ans_plus = cJSON_CreateNumber(ans + 1.0);
    cJSON_AddItemToObject(root, "answer_plus_one", ans_plus);

    // step 3: Operate
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON *second = cJSON_CreateNumber(100.0);
    cJSON_InsertItemInArray(got_list, 1, second);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_false = cJSON_IsFalse(got_active);
    cJSON_AddBoolToObject(root, "active_flag_copy", is_false);

    // step 4: Cleanup and validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}