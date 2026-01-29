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
//<ID> 1173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *str_array = NULL;
    cJSON *greeting = NULL;
    cJSON *num_item = NULL;
    cJSON *copied = NULL;
    cJSON *echo_num = NULL;
    char *printed = NULL;
    const char *retrieved = NULL;
    const char *names[] = { "alpha", "beta", "gamma" };
    double updated_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(names, 3);
    greeting = cJSON_CreateString("welcome");
    num_item = cJSON_CreateNumber(1.23);
    cJSON_AddItemToObject(root, "names", str_array);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "number", num_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetStringValue(cJSON_GetArrayItem(str_array, 1));
    copied = cJSON_CreateString(retrieved);
    cJSON_AddItemToObject(root, "copied_name", copied);
    updated_value = cJSON_SetNumberHelper(num_item, 42.0);
    echo_num = cJSON_CreateNumber(updated_value);
    cJSON_AddItemToObject(root, "echo_number", echo_num);
    cJSON_DeleteItemFromArray(str_array, 0);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}