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
//<ID> 625
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
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *arr = cJSON_CreateArray();
    cJSON *str1 = cJSON_CreateString("first");
    cJSON *str2 = cJSON_CreateString("second");
    cJSON *str_repl = cJSON_CreateString("replaced");
    cJSON *got_meta = NULL;
    size_t root_len = 0;
    size_t to_copy_root = 0;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddStringToObject(meta, "info", "example");
    got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_AddStringToObject(got_meta, "note", "retrieved");
    cJSON_ReplaceItemInArray(arr, 0, str_repl);

    // step 3: Operate / Validate
    printed_root = cJSON_PrintUnformatted(root);
    root_len = printed_root ? strlen(printed_root) : 0;
    to_copy_root = root_len < 511 ? root_len : 511;
    memcpy(scratch, printed_root ? printed_root : "", to_copy_root);
    scratch[to_copy_root] = '\0';
    arr_size = cJSON_GetArraySize(arr);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}