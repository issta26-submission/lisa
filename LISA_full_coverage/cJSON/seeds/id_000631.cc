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
//<ID> 631
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_fragment = "{\"raw_key\":[1,2,3]}";
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_AddObjectToObject(root, "objA");
    cJSON_AddRawToObject(objA, "r", raw_fragment);
    cJSON *objB = cJSON_Parse("{\"objA\":{\"r\":{\"raw_key\":[1,2,3]}}}");

    // step 2: Configure
    cJSON_bool is_obj = cJSON_IsObject(objA);
    cJSON_bool are_equal = cJSON_Compare(root, objB, 1);

    // step 3: Operate
    cJSON_AddBoolToObject(root, "is_obj", is_obj);
    cJSON_AddNumberToObject(root, "equal", are_equal ? 1.0 : 0.0);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(objB);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}