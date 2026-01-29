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
//<ID> 1320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup_root = NULL;
    const char *version_str = NULL;
    cJSON_bool cmp_result = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "greeting", "hello");
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    dup_root = cJSON_Duplicate(root, 1);
    cmp_result = cJSON_Compare(root, dup_root, 1);
    version_str = cJSON_Version();
    cJSON_AddStringToObject(dup_root, "version", version_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}