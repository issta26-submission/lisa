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
//<ID> 260
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
    cJSON_bool cmp_eq = 0;
    cJSON *first = NULL;
    cJSON *second = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemReferenceToArray(arr, true_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    cmp_eq = cJSON_Compare(first, second, 1);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = cmp_eq ? '1' : '0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}