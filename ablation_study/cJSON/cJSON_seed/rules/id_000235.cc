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
//<ID> 235
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *t = cJSON_CreateTrue();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    int arr_size = 0;
    cJSON *first = NULL;
    cJSON *second = NULL;
    char *str_val = NULL;
    cJSON_bool true_check = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemReferenceToArray(arr, s1);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    str_val = cJSON_GetStringValue(first);
    true_check = cJSON_IsTrue(second);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}