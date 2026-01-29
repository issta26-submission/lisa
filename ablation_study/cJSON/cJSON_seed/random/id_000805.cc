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
//<ID> 805
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_value = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool is_array = 0;
    cJSON_bool is_false = 0;
    char *printed = (char *)0;
    int validation = 0;
    int ints[3] = {10, 20, 30};

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(7.5);
    int_array = cJSON_CreateIntArray(ints, 3);
    flag_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(meta, "list", int_array);
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 4: Operate
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    retrieved_list = cJSON_GetObjectItem(retrieved_meta, "list");
    retrieved_value = cJSON_GetObjectItem(root, "value");
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    is_array = cJSON_IsArray(retrieved_list);
    is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation = (retrieved_meta != (cJSON *)0) + (retrieved_list != (cJSON *)0) + (retrieved_value != (cJSON *)0) + (retrieved_flag != (cJSON *)0) + (int)is_array + (int)is_false + (printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}