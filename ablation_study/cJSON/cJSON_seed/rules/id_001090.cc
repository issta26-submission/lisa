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
//<ID> 1090
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
    cJSON *greet = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *got = NULL;
    char *printed = NULL;
    const char *got_greet_str = NULL;
    const char *got_arr0_str = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "items", arr);
    greet = cJSON_CreateString("hello");
    cJSON_AddItemToObjectCS(root, "greeting", greet);
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemReferenceToArray(arr, s1);
    cJSON_AddItemReferenceToArray(arr, s2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    got = cJSON_GetArrayItem(arr, 0);
    got_arr0_str = cJSON_GetStringValue(got);
    got_greet_str = cJSON_GetStringValue(greet);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}