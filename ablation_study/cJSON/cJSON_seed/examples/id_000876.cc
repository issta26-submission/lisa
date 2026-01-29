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
//<ID> 876
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("device-007");
    cJSON *count_item = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "count", count_item);

    // step 3: Operate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    cJSON_bool name_invalid = cJSON_IsInvalid(got_name);
    cJSON_bool count_is_number = cJSON_IsNumber(got_count);
    const char *name_str = cJSON_GetStringValue(got_name);
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON_AddStringToObject(root, "name_copy", name_str);
    cJSON_AddNumberToObject(root, "count_copy", count_val);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}