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
//<ID> 664
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = cJSON_Version();
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s_first = cJSON_CreateString("first_item");
    cJSON *n_pi = cJSON_CreateNumber(3.14159);
    cJSON *false_item = NULL;
    char *printed = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    int arr_size_before = 0;
    const cJSON *peek_item = NULL;
    const char *peek_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "items", arr);
    false_item = cJSON_AddFalseToObject(root, "disabled");
    cJSON_AddItemToArray(arr, s_first);
    cJSON_AddItemToArray(arr, n_pi);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    arr_size_before = cJSON_GetArraySize(arr);
    peek_item = cJSON_GetArrayItem(arr, 0);
    peek_str = cJSON_GetStringValue(peek_item);
    cJSON_DeleteItemFromArray(arr, 0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}