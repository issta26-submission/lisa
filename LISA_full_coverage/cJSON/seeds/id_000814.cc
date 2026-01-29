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
//<ID> 814
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(0.0);
    double set_val = cJSON_SetNumberHelper(num_item, 2021.5);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *desc_item = cJSON_AddStringToObject(meta, "description", "A test metadata");
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "example");

    // step 2: Configure
    cJSON_AddItemToObject(meta, "timestamp", num_item);
    cJSON *nested = cJSON_AddObjectToObject(meta, "nested");
    cJSON_AddStringToObject(nested, "note", "nested note");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    double read_num = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "timestamp"));
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    (void)set_val;
    (void)name_val;
    (void)read_num;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}