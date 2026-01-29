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
//<ID> 257
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
    cJSON *str_ref = cJSON_CreateStringReference("ref_str");
    cJSON *regular_str = cJSON_CreateString("regular");
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int arr_size = 0;
    const char *first_val = NULL;
    double id_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "id", 42.0);
    cJSON_AddNumberToObject(root, "temp", 999.0);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToArray(arr, regular_str);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    id_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "id"));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)arr_size;
    (void)first_val;
    (void)id_value;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}