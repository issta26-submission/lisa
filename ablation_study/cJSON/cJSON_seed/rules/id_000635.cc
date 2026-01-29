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
//<ID> 635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *meta = cJSON_CreateObject();
    char *printed_root = NULL;
    char *printed_elem = NULL;
    cJSON *elem = NULL;
    size_t root_len = 0;
    size_t elem_len = 0;
    size_t to_copy_root = 0;
    size_t to_copy_elem = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "version", cJSON_CreateString("1.0"));

    // step 3: Operate / Validate
    elem = cJSON_GetArrayItem(arr, 1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_elem = cJSON_PrintUnformatted(elem);
    root_len = printed_root ? strlen(printed_root) : 0;
    elem_len = printed_elem ? strlen(printed_elem) : 0;
    to_copy_root = root_len < 255 ? root_len : 255;
    to_copy_elem = (elem_len < (511 - to_copy_root)) ? elem_len : (511 - to_copy_root);
    memcpy(scratch, printed_root ? printed_root : "", to_copy_root);
    scratch[to_copy_root] = '\0';
    memcpy(scratch + to_copy_root, printed_elem ? printed_elem : "", to_copy_elem);
    scratch[to_copy_root + to_copy_elem] = '\0';
    cJSON_free(printed_root);
    cJSON_free(printed_elem);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}