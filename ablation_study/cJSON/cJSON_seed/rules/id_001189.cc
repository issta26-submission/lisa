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
//<ID> 1189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *ok_ptr = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    double answer_value = 0.0;
    cJSON_bool replace_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);
    cJSON_AddTrueToObject(root, "ok");
    ok_ptr = cJSON_GetObjectItem(root, "ok");
    replacement = cJSON_CreateFalse();
    replace_result = cJSON_ReplaceItemViaPointer(root, ok_ptr, replacement);

    // step 3: Operate / Validate
    answer_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    printed = cJSON_PrintUnformatted(root);
    (void)answer_value;
    (void)replace_result;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}