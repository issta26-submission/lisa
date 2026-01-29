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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "meta", meta);
    cJSON *names = cJSON_AddArrayToObject(root, "names");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(names, s1);
    cJSON_AddItemToArray(names, s2);
    cJSON_AddItemToArray(names, s3);
    cJSON_AddItemToObjectCS(meta, "origin", cJSON_CreateString("fuzz"));

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(names, 1);
    const char *second_val = cJSON_GetStringValue(second);
    cJSON *selected = cJSON_CreateString(second_val);
    cJSON_AddItemToObjectCS(root, "selected", selected);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}