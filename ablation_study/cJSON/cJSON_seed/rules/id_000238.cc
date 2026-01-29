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
//<ID> 238
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
    cJSON *str = cJSON_CreateString("fuzz-string");
    cJSON *true_item = cJSON_CreateTrue();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    char *sval = NULL;
    cJSON *first = NULL;
    cJSON *third = NULL;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemReferenceToArray(arr, str);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    third = cJSON_GetArrayItem(arr, 2);
    sval = cJSON_GetStringValue(first);
    arr_size = cJSON_GetArraySize(arr);
    is_true = cJSON_IsTrue(cJSON_GetArrayItem(arr, 1));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)sval;
    (void)arr_size;
    (void)is_true;
    (void)third;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}