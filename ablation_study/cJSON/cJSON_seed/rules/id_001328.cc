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
//<ID> 1328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *dup_root = NULL;
    char *out = NULL;
    int arr_size = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddStringToObject(root, "lib_version", version);
    cJSON_AddStringToObject(root, "status", "initialized");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    dup_root = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup_root, 1);
    out = cJSON_PrintUnformatted(root);
    (void)arr_size;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}