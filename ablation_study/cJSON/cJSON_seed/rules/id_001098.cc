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
//<ID> 1098
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
    cJSON *str_item2 = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    char *got_str = NULL;
    int arr_size = 0;
    cJSON_bool add_res = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "list", arr);
    str_item = cJSON_CreateString("alpha");
    cJSON_AddItemReferenceToArray(arr, str_item);
    str_item2 = cJSON_CreateString("beta");
    cJSON_AddItemReferenceToArray(arr, str_item2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got = cJSON_GetArrayItem(arr, 0);
    got_str = cJSON_GetStringValue(got);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}