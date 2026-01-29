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
//<ID> 822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "version", cJSON_Version());
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", list);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(list, n0);
    cJSON_AddItemToArray(list, s0);
    cJSON *n_insert = cJSON_CreateNumber(1.5);
    cJSON_InsertItemInArray(list, 1, n_insert);

    // step 2: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);

    // step 3: Validate
    cJSON *got = cJSON_GetArrayItem(list, 1);
    double val = cJSON_GetNumberValue(got);
    cJSON_SetNumberHelper(n0, val + 10.0);
    (void)val;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "version");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}