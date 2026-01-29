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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"num\":1.5, \"text\":\"orig\", \"nested\": {\"inner\": 2}}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON *text_item = cJSON_GetObjectItem(root, "text");
    cJSON *nested = cJSON_GetObjectItem(root, "nested");

    // step 2: Configure
    double set_res = cJSON_SetNumberHelper(num_item, 3.14159);
    char *newstr = cJSON_SetValuestring(text_item, "changed");
    cJSON_AddRawToObject(root, "raw", "{\"rawnum\":10}");
    cJSON *nested_dup = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nestedRef", nested_dup);

    // step 3: Operate and Validate
    double val = cJSON_GetNumberValue(num_item);
    char *sval = cJSON_GetStringValue(text_item);
    double derived = val * 2.0 + set_res;
    cJSON_AddNumberToObject(root, "double_num", derived);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_SetNumberHelper(cJSON_GetObjectItem(dup, "num"), 0.0);
    cJSON_SetValuestring(cJSON_GetObjectItem(dup, "text"), "dup_text");
    (void)newstr;
    (void)sval;
    (void)derived;
    (void)set_res;

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}