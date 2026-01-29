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
//<ID> 230
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
    cJSON *str = cJSON_CreateString("fuzz-ref");
    cJSON *tru = cJSON_CreateTrue();
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    const char *s0 = NULL;
    const char *s2 = NULL;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToArray(arr, tru);
    cJSON_AddItemReferenceToArray(arr, str);

    // step 3: Operate / Validate
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    s0 = cJSON_GetStringValue(item0);
    is_true = cJSON_IsTrue(item1);
    s2 = cJSON_GetStringValue(item2);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    (void)s0;
    (void)is_true;
    (void)s2;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}