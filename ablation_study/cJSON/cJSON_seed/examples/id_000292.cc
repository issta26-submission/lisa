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
//<ID> 292
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
    cJSON *name = cJSON_CreateString("example");
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON *active = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON_AddItemToObjectCS(root, "name", name);
    cJSON_AddItemToObjectCS(root, "count", count);
    cJSON_AddItemToObjectCS(root, "active", active);

    // step 3: Operate & Validate
    cJSON_bool has_count = cJSON_HasObjectItem(root, "count");
    cJSON *has_count_item = cJSON_CreateBool(has_count);
    cJSON_AddItemToObjectCS(root, "has_count", has_count_item);
    cJSON *retrieved_count = cJSON_GetObjectItem(root, "count");
    double numeric_value = cJSON_GetNumberValue(retrieved_count);
    cJSON *count_plus_one = cJSON_CreateNumber(numeric_value + 1.0);
    cJSON_AddItemToObjectCS(root, "count_plus_one", count_plus_one);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "active");
    cJSON_bool has_active_after = cJSON_HasObjectItem(root, "active");
    cJSON *has_active_after_item = cJSON_CreateBool(has_active_after);
    cJSON_AddItemToObjectCS(root, "has_active_after_delete", has_active_after_item);

    // step 4: Cleanup
    char *printed = cJSON_Print(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}