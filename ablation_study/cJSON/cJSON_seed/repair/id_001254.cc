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
//<ID> 1254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "[\"alpha\", 10, \"beta\"]";
    cJSON *root = cJSON_ParseWithLength(json, (size_t)strlen(json));
    cJSON *item0 = cJSON_GetArrayItem(root, 0);

    // step 2: Configure
    cJSON *dup0 = cJSON_Duplicate(item0, 1);
    cJSON_SetValuestring(dup0, "alpha-modified");
    cJSON_AddItemToArray(root, dup0);

    // step 3: Operate and Validate
    cJSON *item1 = cJSON_GetArrayItem(root, 1);
    double v1 = cJSON_GetNumberValue(item1);
    cJSON *item2 = cJSON_GetArrayItem(root, 2);
    const char *s2 = cJSON_GetStringValue(item2);
    size_t s2len = strlen(s2);
    double computed = v1 + (double)s2len;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToArray(root, computed_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}