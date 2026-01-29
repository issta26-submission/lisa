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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_main = "{\"count\":2,\"name\":\"example\"}";
    cJSON *root = cJSON_Parse(json_main);
    const char *json_sub = "{\"x\":1}";
    cJSON *sub = cJSON_Parse(json_sub);

    // step 2: Configure
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *added = cJSON_AddNumberToObject(root, "added", 42.0);

    // step 3: Operate and Validate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *count_times_two = cJSON_AddNumberToObject(root, "count_times_two", count_val * 2.0);
    cJSON *sub_obj = cJSON_GetObjectItem(root, "sub");
    cJSON *x_item = cJSON_GetObjectItem(sub_obj, "x");
    double x_val = cJSON_GetNumberValue(x_item);
    cJSON_AddNumberToObject(root, "x_plus_added", x_val + cJSON_GetNumberValue(added));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}