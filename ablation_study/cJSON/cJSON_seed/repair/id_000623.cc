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
//<ID> 623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 5.0);
    cJSON_AddNullToObject(root, "missing");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddBoolToObject(root, "flag_is_bool", flag_is_bool);

    // step 3: Operate and Validate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double doubled = count_val * 2.0;
    cJSON_AddNumberToObject(root, "double_count", doubled);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}