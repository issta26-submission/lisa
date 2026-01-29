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
//<ID> 624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *replacement = NULL;
    cJSON *fetched_list = NULL;
    cJSON *first_item = NULL;
    const char *first_value = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    child = cJSON_AddObjectToObject(root, "child");
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 0, replacement);
    fetched_list = cJSON_GetObjectItem(root, "list");
    first_item = cJSON_GetArrayItem(fetched_list, 0);
    first_value = cJSON_GetStringValue(first_item);
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy < 512 ? to_copy : 511] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}