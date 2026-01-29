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
//<ID> 234
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
    cJSON *child = cJSON_CreateObject();
    cJSON *s = cJSON_CreateString("example");
    cJSON *f = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "name", s);
    cJSON_AddItemToObject(child, "active", f);

    // step 2: Configure
    cJSON_InsertItemInArray(array, 0, child);
    cJSON_AddItemToObject(root, "items", array);

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItem(root, "items");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(retrieved, 1, num);
    double count = (double)cJSON_GetArraySize(retrieved);
    cJSON_AddNumberToObject(root, "count", count);

    // step 4: Cleanup and validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}