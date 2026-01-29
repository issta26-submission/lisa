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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *inner_num = cJSON_CreateNumber(42.0);
    cJSON *label = cJSON_CreateString("demo_label");

    // step 2: Configure
    cJSON_AddItemToObjectCS(child, "inner_num", inner_num);
    cJSON_AddItemToObjectCS(child, "label", label);
    cJSON_AddItemToObjectCS(root, "child", child);

    // step 3: Operate & Validate
    cJSON_bool has_before = cJSON_HasObjectItem(root, "child");
    cJSON *has_before_flag = cJSON_CreateBool(has_before);
    cJSON_AddItemToObjectCS(root, "has_child_before", has_before_flag);
    cJSON *retrieved_child = cJSON_GetObjectItem(root, "child");
    cJSON *retrieved_inner = cJSON_GetObjectItem(retrieved_child, "inner_num");
    double inner_value = cJSON_GetNumberValue(retrieved_inner);
    cJSON *copied_value = cJSON_CreateNumber(inner_value);
    cJSON_AddItemToObjectCS(root, "copied_inner_num", copied_value);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "child");
    cJSON_bool has_after = cJSON_HasObjectItem(root, "child");
    cJSON *has_after_flag = cJSON_CreateBool(has_after);
    cJSON_AddItemToObjectCS(root, "has_child_after", has_after_flag);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}