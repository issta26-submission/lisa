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
//<ID> 818
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *count = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(meta, "count", count);
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "example_name");

    // step 2: Configure
    double set_result = cJSON_SetNumberHelper(count, 42.0);
    (void)set_result;

    // step 3: Operate & Validate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON *retrieved_count = cJSON_GetObjectItem(meta, "count");
    double retrieved_value = cJSON_GetNumberValue(retrieved_count);
    cJSON *retrieved_name_item = cJSON_GetObjectItem(meta, "name");
    char *retrieved_name = cJSON_GetStringValue(retrieved_name_item);
    (void)retrieved_value;
    (void)retrieved_name;

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}