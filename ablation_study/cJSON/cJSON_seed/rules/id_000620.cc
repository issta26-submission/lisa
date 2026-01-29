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
//<ID> 620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *s3 = cJSON_CreateString("three");
    cJSON *replacement = NULL;
    cJSON *got_child = NULL;
    size_t root_len = 0;
    size_t to_copy = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(child, "array", arr);
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("example"));

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    got_child = cJSON_GetObjectItem(root, "child");
    printed_root = cJSON_PrintUnformatted(root);
    root_len = printed_root ? strlen(printed_root) : 0;
    to_copy = root_len < 511 ? root_len : 511;
    memcpy(scratch, printed_root ? printed_root : "", to_copy);
    scratch[to_copy] = '\0';
    arr_size = cJSON_GetArraySize(arr);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}