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
//<ID> 1097
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *elem1 = NULL;
    cJSON *ref = NULL;
    cJSON *direct = NULL;
    cJSON *first = NULL;
    char *printed = NULL;
    char *got_string = NULL;
    int arr_size = 0;
    cJSON_bool res1 = 0;
    cJSON_bool res2 = 0;
    cJSON_bool res3 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    res1 = cJSON_AddItemToObjectCS(root, "list", arr);
    elem1 = cJSON_CreateString("first_element");
    cJSON_AddItemToArray(arr, elem1);
    ref = cJSON_CreateString("referenced_element");
    res3 = cJSON_AddItemReferenceToArray(arr, ref);
    direct = cJSON_CreateString("direct_value");
    res2 = cJSON_AddItemToObjectCS(root, "direct", direct);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    got_string = cJSON_GetStringValue(first);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}