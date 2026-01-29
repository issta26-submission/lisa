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
//<ID> 1194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *other = cJSON_CreateObject();
    const char *json = "{\"p\":3}";
    const char *endptr = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, (size_t)7, &endptr, 1);
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddNullToObject(root, "maybe");
    cJSON *str = cJSON_AddStringToObject(root, "greet", "hello");

    // step 2: Configure
    cJSON *numobj = cJSON_CreateNumber(0.0);
    double setret = cJSON_SetNumberHelper(numobj, 4.5);
    cJSON_AddItemToObject(root, "setnum", numobj);
    double parsed_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "p"));
    double final_val = setret + parsed_val;
    cJSON_AddNumberToObject(root, "final", final_val);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, str);
    cJSON_AddItemToObject(other, "moved", detached);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(other);

    // API sequence test completed successfully
    return 66;
}