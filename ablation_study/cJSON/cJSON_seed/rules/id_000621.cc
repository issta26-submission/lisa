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
//<ID> 621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_before = NULL;
    char *printed_after = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_AddObjectToObject(root, "inner");
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    size_t before_len = 0;
    size_t after_len = 0;
    size_t to_copy_before = 0;
    size_t to_copy_after = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(inner, "items", arr);
    cJSON_AddStringToObject(root, "title", "example");

    // step 3: Operate / Validate
    printed_before = cJSON_PrintUnformatted(root);
    cJSON *found_inner = cJSON_GetObjectItem(root, "inner");
    cJSON *found_items = cJSON_GetObjectItem(found_inner, "items");
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(found_items, 1, replacement);
    printed_after = cJSON_PrintUnformatted(root);
    before_len = printed_before ? strlen(printed_before) : 0;
    after_len = printed_after ? strlen(printed_after) : 0;
    to_copy_before = before_len < 255 ? before_len : 255;
    to_copy_after = (after_len < (511 - to_copy_before)) ? after_len : (511 - to_copy_before);
    memcpy(scratch, printed_before ? printed_before : "", to_copy_before);
    scratch[to_copy_before] = '\0';
    memcpy(scratch + to_copy_before, printed_after ? printed_after : "", to_copy_after);
    scratch[to_copy_before + to_copy_after] = '\0';
    arr_size = cJSON_GetArraySize(found_items);

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}