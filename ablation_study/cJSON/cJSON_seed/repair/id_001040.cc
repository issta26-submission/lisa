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
//<ID> 1040
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", array);
    cJSON *f = cJSON_AddFalseToObject(root, "flag");
    cJSON *n = cJSON_AddNullToObject(root, "nothing");
    const char *version_const = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version_const);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    char *flat = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    cJSON *got_array = cJSON_GetObjectItem(root, "array");
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    char *vstr = cJSON_GetStringValue(got_version);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = vstr ? vstr[0] : '\0';
    ((char *)scratch)[1] = (char)('0' + (int)cJSON_IsArray(got_array));

    // step 4: Cleanup
    cJSON_free(flat);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}