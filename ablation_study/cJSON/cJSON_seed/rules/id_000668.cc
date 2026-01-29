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
//<ID> 668
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *n2 = cJSON_CreateNumber(2.0);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *false_item = cJSON_AddFalseToObject(root, "enabled");
    cJSON_AddStringToObject(root, "tag", "alpha");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    const char *ver = cJSON_Version();
    size_t ver_len = ver ? strlen(ver) : 0;
    size_t ver_to_copy = ver_len < (255 - to_copy) ? ver_len : (255 - to_copy);
    memcpy(scratch + to_copy, ver ? ver : "", ver_to_copy);
    scratch[to_copy + ver_to_copy] = '\0';
    int arr_size_before = cJSON_GetArraySize(arr);
    cJSON_DeleteItemFromArray(arr, 0);
    int arr_size_after = cJSON_GetArraySize(arr);
    cJSON *remaining = cJSON_GetArrayItem(arr, 0);
    const char *remaining_str = cJSON_GetStringValue(remaining);
    double remaining_num = cJSON_GetNumberValue(remaining);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}