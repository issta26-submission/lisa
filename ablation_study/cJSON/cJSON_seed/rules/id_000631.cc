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
//<ID> 631
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed_root = NULL;
    char *printed_elem = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *meta = cJSON_CreateObject();
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *elem = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    elem = cJSON_GetArrayItem(arr, 1);
    printed_elem = cJSON_PrintUnformatted(elem);
    printed_root = cJSON_PrintUnformatted(root);
    printed_len = printed_root ? strlen(printed_root) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed_root ? printed_root : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed_elem);
    cJSON_free(printed_root);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}