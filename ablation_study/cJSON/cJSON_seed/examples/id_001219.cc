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
//<ID> 1219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[10,20,30],\"remove_me\":999,\"meta\":{\"factor\":2}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *numbers_ref = cJSON_CreateArrayReference(numbers);
    cJSON_AddItemToObject(root, "migrated_numbers", numbers_ref);
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *factor_item = cJSON_GetObjectItem(meta, "factor");
    double factor = cJSON_GetNumberValue(factor_item);

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *computed = cJSON_CreateNumber(second_val * factor);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON_DeleteItemFromObject(parsed, "remove_me");

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}