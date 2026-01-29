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
//<ID> 1114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old\",\"count\":5,\"flag\":false}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *prev_name = cJSON_SetValuestring(name_item, "new_name");
    char *current_name = cJSON_GetStringValue(name_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double computed = count_val + (double)current_name[0];
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "flag");
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_free(unformatted);
    cJSON_Delete(root);

    (void)prev_name; // silence unused-variable warnings

    // API sequence test completed successfully
    return 66;
}