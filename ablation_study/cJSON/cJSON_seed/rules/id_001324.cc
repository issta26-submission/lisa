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
//<ID> 1324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"numbers\": [1, 2, 3], \"name\": \"original\"}";
    const char *ver = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *meta = NULL;
    cJSON *dup = NULL;
    int arr_size = 0;
    cJSON_bool eq = 0;

    // step 2: Setup / Configure
    ver = cJSON_Version();
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(arr);
    meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", (double)arr_size);
    cJSON_AddStringToObject(meta, "version", ver);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    dup = cJSON_Duplicate(root, 1);
    eq = cJSON_Compare(root, dup, 1);
    (void)arr_size;
    (void)eq;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}