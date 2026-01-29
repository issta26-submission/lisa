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
//<ID> 477
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"obj\":{\"val\":2.5},\"vals\":[1.0,2.0]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *val = cJSON_GetObjectItem(obj, "val");
    cJSON *vals = cJSON_GetObjectItem(root, "vals");
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(vals, 0));

    // step 2: Configure
    cJSON *refs = cJSON_AddArrayToObject(root, "refs");
    cJSON *enabled = cJSON_AddTrueToObject(root, "enabled");
    cJSON_bool ref_added = cJSON_AddItemReferenceToArray(refs, obj);

    // step 3: Operate
    double new_number = cJSON_SetNumberHelper(val, cJSON_GetNumberValue(val) + first_val + (double)cJSON_IsObject(obj));
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)enabled;
    (void)ref_added;
    (void)new_number;
    (void)first_val;
    return 66;
}