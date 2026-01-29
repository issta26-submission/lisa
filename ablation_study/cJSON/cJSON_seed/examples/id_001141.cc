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
//<ID> 1141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_array[] = "[10,20,30]";
    const char json_obj[] = "{\"name\":\"alpha\"}";
    cJSON *root_array = cJSON_ParseWithLength(json_array, sizeof(json_array) - 1);
    cJSON *root_obj = cJSON_Parse(json_obj);

    // step 2: Configure
    cJSON *elem = cJSON_GetArrayItem(root_array, 1);
    double val = cJSON_GetNumberValue(elem);
    cJSON *doubled = cJSON_CreateNumber(val * 2.0);
    cJSON *nil = cJSON_CreateNull();
    cJSON_AddItemToObject(root_obj, "doubled", doubled);
    cJSON_AddItemToObject(root_obj, "extra_null", nil);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root_obj);
    cJSON *retrieved = cJSON_GetObjectItem(root_obj, "doubled");
    double retrieved_val = cJSON_GetNumberValue(retrieved);
    (void)retrieved_val;
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root_array);
    cJSON_Delete(root_obj);
    // API sequence test completed successfully
    return 66;
}