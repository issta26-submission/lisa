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
//<ID> 1218
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
    cJSON *str1 = NULL;
    cJSON *str2 = NULL;
    cJSON *copied = NULL;
    cJSON *second_item = NULL;
    char *printed = NULL;
    const char *second_val = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);

    // step 3: Operate / Validate
    second_item = cJSON_GetArrayItem(arr, 1);
    second_val = cJSON_GetStringValue(second_item);
    copied = cJSON_CreateString(second_val);
    cJSON_AddItemToObject(root, "second_copy", copied);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}