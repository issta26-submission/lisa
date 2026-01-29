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
//<ID> 1277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "config");
    cJSON *array = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");

    // step 2: Configure
    cJSON_AddItemToObject(nested, "first", s1);
    cJSON_AddNullToObject(nested, "nothing");
    cJSON_AddItemToObject(nested, "list", array);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, s3);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(array);
    cJSON_AddNumberToObject(root, "count", (double)size);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}