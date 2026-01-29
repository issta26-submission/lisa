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
//<ID> 1448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "cjson-sequence");
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");

    // step 2: Configure
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToArray(flags, cJSON_CreateFalse());
    cJSON_AddItemToArray(flags, cJSON_CreateTrue());
    cJSON_AddItemToArray(flags, cJSON_CreateFalse());

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(flags, 0);
    cJSON *second = cJSON_GetArrayItem(flags, 1);
    cJSON_bool first_true = cJSON_IsTrue(first);
    cJSON_bool second_true = cJSON_IsTrue(second);
    char *out = cJSON_PrintUnformatted(root);
    size_t len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(len + 16);
    memset(scratch, 0, len + 16);
    scratch[0] = first_true ? '1' : '0';
    scratch[1] = second_true ? '1' : '0';
    scratch[2] = len > 0 ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}