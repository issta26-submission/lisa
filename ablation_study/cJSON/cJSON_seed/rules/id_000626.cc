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
//<ID> 626
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *printed_meta = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *meta = NULL;
    cJSON *array_from_root = NULL;
    cJSON *replacement = NULL;
    size_t root_len = 0;
    size_t meta_len = 0;
    size_t to_copy_root = 0;
    size_t to_copy_meta = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "array", arr);
    meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "status", "active");

    // step 3: Operate / Validate
    array_from_root = cJSON_GetObjectItem(root, "array");
    replacement = cJSON_CreateString("zero");
    cJSON_ReplaceItemInArray(array_from_root, 0, replacement);
    printed_root = cJSON_PrintUnformatted(root);
    printed_meta = cJSON_PrintUnformatted(meta);
    root_len = printed_root ? strlen(printed_root) : 0;
    meta_len = printed_meta ? strlen(printed_meta) : 0;
    to_copy_root = root_len < 255 ? root_len : 255;
    to_copy_meta = (meta_len < (511 - to_copy_root)) ? meta_len : (511 - to_copy_root);
    memcpy(scratch, printed_root ? printed_root : "", to_copy_root);
    scratch[to_copy_root] = '\0';
    memcpy(scratch + to_copy_root, printed_meta ? printed_meta : "", to_copy_meta);
    scratch[to_copy_root + to_copy_meta] = '\0';

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_meta);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}