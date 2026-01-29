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
//<ID> 888
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *value = cJSON_CreateNumber(3.5);
    cJSON *enabled = cJSON_CreateBool((cJSON_bool)1);
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "value", value);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddItemToObject(root, "meta", meta);
    double raw_value = cJSON_GetNumberValue(value);
    cJSON *double_value = cJSON_CreateNumber(raw_value * 2.0);
    cJSON_AddItemToObjectCS(meta, "double", double_value);

    // step 3: Operate & Validate
    cJSON *got_value = cJSON_GetObjectItemCaseSensitive(root, "value");
    double extracted = cJSON_GetNumberValue(got_value);
    cJSON_bool is_number = cJSON_IsNumber(got_value);
    cJSON *is_number_flag = cJSON_CreateBool(is_number);
    cJSON_AddItemToObject(root, "value_is_number", is_number_flag);
    cJSON *computed = cJSON_CreateNumber(extracted + cJSON_GetNumberValue(double_value));
    cJSON_AddItemToObjectCS(root, "sum", computed);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}