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
//<ID> 1179
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
    cJSON *s1 = NULL;
    cJSON *s_from_first = NULL;
    cJSON *num = NULL;
    const char *list[] = { "alpha", "beta", "gamma" };
    const char *first_str = NULL;
    char *printed = NULL;
    double setnum_ret = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateStringArray(list, 3);
    cJSON_AddItemToObject(root, "list", arr);
    s1 = cJSON_CreateString("delta");
    cJSON_AddItemToArray(arr, s1);

    // step 3: Operate / Validate
    first_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 0));
    s_from_first = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", s_from_first);
    num = cJSON_CreateNumber(0.0);
    setnum_ret = cJSON_SetNumberHelper(num, 3.1415);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_DeleteItemFromArray(arr, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)setnum_ret;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}