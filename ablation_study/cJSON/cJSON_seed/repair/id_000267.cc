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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\": 42.5, \"name\": \"original\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);

    // step 2: Configure
    cJSON *added = cJSON_CreateString("added_string");
    cJSON_AddItemToObject(root, "added", added);

    // step 3: Operate and Validate
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    double val = cJSON_GetNumberValue(val_item);
    cJSON *newnum = cJSON_CreateNumber(val * 2.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "value", newnum);

    // step 4: Cleanup
    char *out = cJSON_Print(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)parse_end;
    return 66;
}