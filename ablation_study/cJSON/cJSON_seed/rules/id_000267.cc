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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    int arr_size = 0;
    cJSON_bool cmp = 0;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(arr, true_item);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    cmp = cJSON_Compare(true_item, cJSON_GetArrayItem(arr, 0), 1);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = cmp ? '1' : '0';
    (void)arr_size;
    (void)cmp;
    (void)buffer;

    // step 4: Cleanup
    cJSON_Delete(arr);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}