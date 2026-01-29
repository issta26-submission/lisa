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
//<ID> 792
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
    cJSON *raw_item = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    int arr_size = 0;
    const char *version = NULL;
    char *version_copy = NULL;
    size_t version_len = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "arr", arr);
    raw_item = cJSON_CreateRaw("{\"note\":\"this_is_raw\"}");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    version = cJSON_Version();
    version_len = strlen(version);
    version_copy = (char *)cJSON_malloc(version_len + 1);
    memset(version_copy, 0, version_len + 1);
    memcpy(version_copy, version, version_len + 1);

    // step 4: Cleanup
    cJSON_free(version_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}