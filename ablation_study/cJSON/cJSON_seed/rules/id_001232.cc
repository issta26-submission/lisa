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
//<ID> 1232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *sub = NULL;
    cJSON *num_item = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *flag = NULL;
    cJSON *got_sub = NULL;
    cJSON *got_num = NULL;
    cJSON *got_arr = NULL;
    char *printed = NULL;
    double before_value = 0.0;
    double after_value = 0.0;
    cJSON_bool flag_is_false = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    sub = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(sub, "value", num_item);
    cJSON_AddItemToObject(root, "sub", sub);
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("referenced_string");
    cJSON_AddItemReferenceToArray(arr, str_item);
    cJSON_AddItemToObject(root, "list", arr);
    flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    got_sub = cJSON_GetObjectItem(root, "sub");
    got_num = cJSON_GetObjectItem(got_sub, "value");
    before_value = cJSON_GetNumberValue(got_num);
    after_value = cJSON_SetNumberHelper(got_num, 2.71828);
    got_arr = cJSON_GetObjectItem(root, "list");
    flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    printed = cJSON_PrintUnformatted(root);
    (void)before_value;
    (void)after_value;
    (void)got_arr;
    (void)flag_is_false;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}