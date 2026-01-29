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
//<ID> 1095
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
    cJSON *str_item = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;
    const char *got_str = NULL;
    int arr_size = 0;
    cJSON_bool add_res = 0;
    cJSON_bool add_ref_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_res = cJSON_AddItemToObjectCS(root, "items", arr);
    str_item = cJSON_CreateString("hello world");
    add_res = cJSON_AddItemToObjectCS(root, "greeting", str_item);

    // step 3: Operate / Validate
    add_ref_res = cJSON_AddItemReferenceToArray(arr, str_item);
    arr_size = cJSON_GetArraySize(arr);
    got_item = cJSON_GetArrayItem(arr, 0);
    got_str = cJSON_GetStringValue(got_item);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}