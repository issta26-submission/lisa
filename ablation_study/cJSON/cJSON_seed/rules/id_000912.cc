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
//<ID> 912
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *name_ref = NULL;
    cJSON *retrieved_arr = NULL;
    cJSON *retrieved_name = NULL;
    char *printed = NULL;
    char *buffer = NULL;
    int arr_size = 0;
    const int buf_len = 64;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    item0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, item0);
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item1);
    name_ref = cJSON_CreateStringReference("external_name");
    cJSON_AddItemToObject(root, "name", name_ref);

    // step 3: Operate / Validate
    retrieved_arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    printed = cJSON_PrintUnformatted(root);
    buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (arr_size % 10));
    (void)retrieved_name;
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}