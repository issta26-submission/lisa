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
//<ID> 1266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"embedded\":42}";
    const char raw_inside[] = "{\"inside\":true}";
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = NULL;
    cJSON *raw_item = NULL;
    cJSON *count_item = NULL;
    cJSON *retrieved_raw = NULL;
    cJSON *retrieved_count = NULL;
    cJSON *retrieved_valid = NULL;
    cJSON *double_item = NULL;
    char *printed = NULL;
    const char *raw_str = NULL;
    double count_value = 0.0;
    cJSON_bool valid_flag = 0;

    // step 2: Setup / Configure
    meta = cJSON_AddObjectToObject(root, "meta");
    raw_item = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObjectCS(root, "raw_payload", raw_item);
    cJSON_AddTrueToObject(meta, "valid");
    cJSON_AddItemToObjectCS(meta, "raw_inside", cJSON_CreateRaw(raw_inside));
    count_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObjectCS(root, "count", count_item);

    // step 3: Operate / Validate
    retrieved_raw = cJSON_GetObjectItem(root, "raw_payload");
    retrieved_count = cJSON_GetObjectItem(root, "count");
    retrieved_valid = cJSON_GetObjectItem(meta, "valid");
    raw_str = cJSON_GetStringValue(retrieved_raw);
    count_value = cJSON_GetNumberValue(retrieved_count);
    valid_flag = cJSON_IsTrue(retrieved_valid);
    double_item = cJSON_CreateNumber(count_value * 2.0);
    cJSON_AddItemToObjectCS(meta, "double_count", double_item);
    printed = cJSON_PrintUnformatted(root);
    (void)raw_str;
    (void)valid_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}