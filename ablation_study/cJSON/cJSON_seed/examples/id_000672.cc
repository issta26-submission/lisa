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
//<ID> 672
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddNumberToObject(root, "scalar", 3.14);

    // step 2: Configure
    cJSON *scalar = cJSON_GetObjectItem(root, "scalar");
    cJSON_SetNumberHelper(scalar, 6.28);
    cJSON_AddItemReferenceToArray(arr, scalar);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_arr = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    int arr_size = cJSON_GetArraySize(parsed_arr);
    cJSON *first = cJSON_GetArrayItem(parsed_arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *parsed_scalar = cJSON_GetObjectItem(parsed, "scalar");
    double scalar_val = cJSON_GetNumberValue(parsed_scalar);
    (void)version;
    (void)arr_size;
    (void)first_val;
    (void)scalar_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}