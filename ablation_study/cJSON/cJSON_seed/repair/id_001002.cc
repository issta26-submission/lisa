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
//<ID> 1002
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "proto", "v1");
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "field", "initial");

    // step 2: Configure
    cJSON *field = cJSON_GetObjectItemCaseSensitive(data, "field");
    char *set_ret = cJSON_SetValuestring(field, "updated_value");
    cJSON_AddStringToObject(root, "setter", set_ret);
    cJSON_AddStringToObject(root, "mirror", cJSON_GetStringValue(field));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_AddStringToObject(root, "dump", printed);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}