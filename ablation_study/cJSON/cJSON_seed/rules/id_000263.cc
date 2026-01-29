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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    int arr_size = 0;
    cJSON *first_elem = NULL;
    cJSON_bool compare_result = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flags", arr);
    cJSON_AddItemReferenceToArray(arr, true_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    compare_result = cJSON_Compare(true_item, first_elem, 1);
    buffer[0] = (char)('0' + (int)compare_result);
    buffer[1] = (char)('0' + (arr_size & 0xF));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(true_item);

    // API sequence test completed successfully
    return 66;
}