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
//<ID> 639
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_AddObjectToObject(root, "objA");
    cJSON_AddNumberToObject(objA, "value", 42.0);

    // step 2: Configure
    cJSON_AddRawToObject(objA, "raw", "{\"inner\":true}");
    cJSON *objB = cJSON_AddObjectToObject(root, "objB");
    cJSON_AddNumberToObject(objB, "value", 42.0);

    // step 3: Operate
    cJSON *dupA = cJSON_Duplicate(objA, 1);
    cJSON_bool equal = cJSON_Compare(objA, dupA, 1);
    cJSON_AddBoolToObject(root, "equal", equal);
    cJSON_bool objB_is_object = cJSON_IsObject(objB);
    cJSON_AddBoolToObject(root, "objB_is_object", objB_is_object);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(dupA);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}