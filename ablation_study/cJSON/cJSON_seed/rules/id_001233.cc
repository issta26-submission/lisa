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
//<ID> 1233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *cfg = NULL;
    cJSON *arr = NULL;
    cJSON *num = NULL;
    cJSON *greet = NULL;
    cJSON *flag = NULL;
    cJSON *retrieved_cfg = NULL;
    cJSON *retrieved_ans = NULL;
    char *printed = NULL;
    double value_after_set = 0.0;
    int list_size = 0;
    cJSON_bool flag_is_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cfg = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num, 3.14159);
    greet = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, greet);
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_AddItemToObject(cfg, "answer", num);
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON_AddItemToObject(root, "list", arr);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    retrieved_cfg = cJSON_GetObjectItem(root, "config");
    retrieved_ans = cJSON_GetObjectItem(retrieved_cfg, "answer");
    value_after_set = cJSON_SetNumberHelper(retrieved_ans, 2.71828);
    flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    list_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "list"));
    printed = cJSON_PrintUnformatted(root);
    (void)value_after_set;
    (void)list_size;
    (void)flag_is_false;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}