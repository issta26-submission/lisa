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
//<ID> 1716
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *data = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_data = (cJSON *)0;
    cJSON *retrieved_num_cs = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    char *json_unformatted = (char *)0;
    double value_num = 0.0;
    char *value_str = (char *)0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    data = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("answer");

    // step 3: Configure
    cJSON_AddItemToObject(data, "number", num_item);
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 4: Operate
    json_unformatted = cJSON_PrintUnformatted(root);
    retrieved_data = cJSON_GetObjectItem(root, "data");
    retrieved_num_cs = cJSON_GetObjectItemCaseSensitive(retrieved_data, "number");
    retrieved_str = cJSON_GetObjectItemCaseSensitive(root, "label");
    value_num = cJSON_GetNumberValue(retrieved_num_cs);
    value_str = cJSON_GetStringValue(retrieved_str);

    // step 5: Validate
    validation = (int)(value_num == 42.0) + (int)(json_unformatted != (char *)0) + (int)(retrieved_data != (cJSON *)0) + (int)(retrieved_num_cs != (cJSON *)0) + (int)(value_str != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)data;
    (void)num_item;
    (void)str_item;
    (void)retrieved_data;
    (void)retrieved_num_cs;
    (void)retrieved_str;
    (void)value_num;
    (void)value_str;
    (void)json_unformatted;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}