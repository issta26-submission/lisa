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
//<ID> 253
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
    cJSON *num_item = cJSON_CreateNumber(100.0);
    const char *ref_str = "example_ref";
    cJSON *str_ref = cJSON_CreateStringReference(ref_str);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int arr_size = 0;
    cJSON *first_elem = NULL;
    double const_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddNumberToObject(root, "constant", 42.5);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_ref);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    const_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "constant"));
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + (arr_size % 10));
    buffer[2] = (char)('0' + ((int)const_value % 10));

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "constant");
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}