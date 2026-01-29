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
//<ID> 610
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
    cJSON *number_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "value", number_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_false", false_item);

    // step 2: Configure
    cJSON_AddNullToObject(root, "optional_field");

    // step 3: Operate & Validate
    cJSON *retrieved_number = cJSON_GetObjectItem(root, "value");
    double numeric_value = cJSON_GetNumberValue(retrieved_number);
    cJSON *retrieved_flag = cJSON_GetObjectItem(root, "flag_false");
    cJSON_bool flag_is_false = cJSON_IsFalse(retrieved_flag);
    char *snapshot = cJSON_PrintUnformatted(root);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;value=%.3f;flag_false=%d",
            version ? version : "null",
            numeric_value,
            (int)flag_is_false);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}