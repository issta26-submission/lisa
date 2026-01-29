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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON *arr = cJSON_CreateArray();
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    double before_value = 0.0;
    double after_value = 0.0;
    double helper_ret = 0.0;
    cJSON_bool flag_is_false = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "text", str);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate / Validate
    before_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    helper_ret = cJSON_SetNumberHelper(num, 42.5);
    after_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    printed = cJSON_PrintUnformatted(root);
    ((char *)buffer)[0] = printed[0];
    (void)before_value;
    (void)after_value;
    (void)helper_ret;
    (void)flag_is_false;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}